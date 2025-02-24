import os
import pika
import psycopg2
from celery import Celery

# Configuração do Celery
app = Celery('tasks', broker='pyamqp://admin:admin@localhost//')

# Configuração do PostgreSQL
DB_HOST = "localhost"
DB_NAME = "messages_db"
DB_USER = "user"
DB_PASS = "password"

# Função que processa as mensagens do RabbitMQ em lote
@app.task
def process_batch():
    connection = pika.BlockingConnection(
        pika.ConnectionParameters(host="localhost", credentials=pika.PlainCredentials('admin', 'admin'))
    )
    channel = connection.channel()
    channel.queue_declare(queue="messages")

    messages = []
    for method_frame, properties, body in channel.consume(queue="messages", inactivity_timeout=5):
        if body:
            messages.append(body.decode())
            channel.basic_ack(method_frame.delivery_tag)
        if len(messages) >= 5:  # Processa em lotes de 5
            break

    connection.close()

    if messages:
        insert_into_db(messages)

# Função que insere mensagens no PostgreSQL
def insert_into_db(messages):
    conn = psycopg2.connect(
        dbname=DB_NAME, user=DB_USER, password=DB_PASS, host=DB_HOST
    )
    cur = conn.cursor()
    cur.executemany("INSERT INTO messages (content) VALUES (%s)", [(msg,) for msg in messages])
    conn.commit()
    cur.close()
    conn.close()
