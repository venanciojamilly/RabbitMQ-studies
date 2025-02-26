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

#define BATCH_SIZE 30  // Número de mensagens por lote

void send_batch_messages(amqp_connection_state_t conn) {
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");

    for (int i = 0; i < BATCH_SIZE; i++) {
        char message[50];
        snprintf(message, sizeof(message), "Mensagem %d", i + 1);
        if (i == 4)
        {
            snprintf(message, sizeof(message), "Jamilly - SPLAB");
        }
        

        amqp_bytes_t message_bytes;
        message_bytes.len = strlen(message);
        message_bytes.bytes = message;

        amqp_basic_publish(
            conn,
            1, // Canal
            amqp_cstring_bytes(EXCHANGE),
            amqp_cstring_bytes(ROUTING_KEY),
            0, // Não é obrigatório
            0, // Não é imediato
            &props,
            message_bytes
        );

        printf("Enviado: %s\n", message);
        usleep(100000);  // Simula pequeno delay entre mensagens
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

    amqp_rpc_reply_t login_reply = amqp_login(
        conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, USERNAME, PASSWORD);
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
    amqp_exchange_declare(
        conn, 1, amqp_cstring_bytes(EXCHANGE), amqp_cstring_bytes("direct"),
        0, 0, 0, 0, amqp_empty_table);

    send_batch_messages(conn);

    // Fecha a conexão
    amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(conn);

    return 0;
}
