# 🐰 RabbitMQ Studies

Exploração do RabbitMQ utilizando as linguagens **C** e **Python** (com a biblioteca `pika`). Este repositório contém exemplos práticos de publicação e consumo de mensagens em filas, facilitando o entendimento do funcionamento desse poderoso broker de mensagens.

## Tecnologias Utilizadas

- **RabbitMQ**: Mensageiro assíncrono baseado no protocolo AMQP, utilizado para comunicação entre sistemas distribuídos.
- **C**: Implementação de clientes RabbitMQ utilizando a biblioteca `librabbitmq`, incluindo exemplos com processamento em batches.
- **Python (pika)**: Cliente RabbitMQ assíncrono para Python, simplificando a comunicação com filas de mensagens.
- **Celery**: Framework de tarefas distribuídas para Python, permitindo processamento assíncrono integrado ao RabbitMQ (**em desenvolvimento**).
- **Flask**: Framework web para Python, utilizado para expor serviços que interagem com RabbitMQ e Celery.

## Conteúdo do Repositório

- `rabbitmq_first_steps/` → Primeiros exemplos de uso do RabbitMQ em Python e C.
- `rabbitmq_batches/` → Implementações avançadas incluindo processamento em batches com C e Celery em Python.
  - `rabbit_mq_c/` → Código em C utilizando batch processing com RabbitMQ.
  - `app.py` → Aplicação Flask integrando RabbitMQ e Celery.
  - `celery_tasks.py` → Definição de tarefas Celery para processamento assíncrono (**em construção**).
  - `worker.py` → Worker Celery responsável pelo processamento das mensagens (**em construção**).
  - `docker-compose.yml` → Configuração para subir um ambiente com RabbitMQ, Celery e Flask.
- `README.md` → Guia rápido sobre os estudos com RabbitMQ.

## Como Executar

1. **Subir o ambiente RabbitMQ**:
   ```sh
   docker-compose up -d
   ```

2. **Executar exemplos em Python**:
   ```sh
   python rabbitmq_first_steps/publisher.py
   python rabbitmq_first_steps/consumer.py
   ```

3. **Executar aplicação Flask com Celery**:
   ```sh
   flask --app rabbitmq_batches/app run
   ```

4. **Compilar e rodar exemplos em C**:
   ```sh
   gcc -o producer rabbitmq_batches/rabbit_mq_c/producer.c -lrabbitmq
   ./producer
   ```
   ```sh
   gcc -o consumer rabbitmq_batches/rabbit_mq_c/consumer.c -lrabbitmq
   ./consumer
   ```

## Objetivo

Este repositório tem como propósito aprofundar os conhecimentos sobre filas de mensagens, comunicação assíncrona e integração de serviços, usando RabbitMQ em diferentes linguagens, incluindo processamento em batches, tarefas distribuídas e aplicações web interativas.

🚀 **Contribua!** Caso tenha sugestões ou melhorias, fique à vontade para abrir um PR. Happy coding! 💡

