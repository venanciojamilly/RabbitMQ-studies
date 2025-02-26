#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <unistd.h>

#define HOSTNAME "localhost"  // Altere se necessário
#define PORT 5672
#define USERNAME "admin"
#define PASSWORD "admin"
#define EXCHANGE "direct_exchange"
#define ROUTING_KEY "data_batch"
#define QUEUE_NAME "batch_queue"

#define BATCH_SIZE 40  // Número de mensagens por lote

// Simulação de inserção no banco de dados
void insert_into_db(char messages[BATCH_SIZE][256], int count) {
    printf("\nInserindo lote no banco de dados...\n");
    for (int i = 0; i < count; i++) {
        printf("Banco de Dados <- %s\n", messages[i]);
    }
    printf("Lote inserido com sucesso!\n\n");
}

void consume_messages(amqp_connection_state_t conn) {
    amqp_basic_consume(conn, 1, amqp_cstring_bytes(QUEUE_NAME),
                       amqp_empty_bytes, 0, 1, 0, amqp_empty_table);

    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    char batch_messages[BATCH_SIZE][256];  // Buffer para mensagens
    int batch_count = 0;

    while (1) {
        amqp_maybe_release_buffers(conn);
        res = amqp_consume_message(conn, &envelope, NULL, 0);

        if (res.reply_type == AMQP_RESPONSE_NORMAL) {
            // Armazena a mensagem no buffer
            snprintf(batch_messages[batch_count], sizeof(batch_messages[batch_count]),
                     "%.*s", (int)envelope.message.body.len, (char *)envelope.message.body.bytes);
            batch_count++;

            printf("Recebido: %s\n", batch_messages[batch_count - 1]);

            // Libera a mensagem
            amqp_destroy_envelope(&envelope);

            // Se atingiu o tamanho do lote, insere no BD
            if (batch_count >= BATCH_SIZE) {
                insert_into_db(batch_messages, batch_count);
                batch_count = 0;  // Reinicia o batch
            }
        } else {
            printf("Erro ao consumir mensagem. Tentando novamente...\n");
            sleep(1);
        }
    }
}

int main() {
    amqp_connection_state_t conn;
    amqp_socket_t *socket = NULL;
    int status;

    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    if (!socket) {
        fprintf(stderr, "Erro ao criar socket\n");
        return 1;
    }

    status = amqp_socket_open(socket, HOSTNAME, PORT);
    if (status) {
        fprintf(stderr, "Erro ao conectar ao RabbitMQ\n");
        return 1;
    }

    amqp_rpc_reply_t login_reply = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, USERNAME, PASSWORD);
    if (login_reply.reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "Falha ao autenticar no RabbitMQ\n");
        return 1;
    }

    amqp_channel_open(conn, 1);
    if (amqp_get_rpc_reply(conn).reply_type != AMQP_RESPONSE_NORMAL) {
        fprintf(stderr, "Erro ao abrir canal\n");
        return 1;
    }

    // Declara o exchange
    amqp_exchange_declare(conn, 1, amqp_cstring_bytes(EXCHANGE), amqp_cstring_bytes("direct"),
                          0, 0, 0, 0, amqp_empty_table);

    // Declara e faz bind da fila ao exchange
    amqp_queue_declare(conn, 1, amqp_cstring_bytes(QUEUE_NAME), 0, 0, 0, 1, amqp_empty_table);
    amqp_queue_bind(conn, 1, amqp_cstring_bytes(QUEUE_NAME), amqp_cstring_bytes(EXCHANGE),
                    amqp_cstring_bytes(ROUTING_KEY), amqp_empty_table);

    printf("Esperando mensagens...\n");
    consume_messages(conn);

    // Fecha conexão (não será executado pois o loop é infinito)
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
