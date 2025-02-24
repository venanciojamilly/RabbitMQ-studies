from flask import Flask, request, jsonify
import pika

app = Flask(__name__)

# Configuração do RabbitMQ
RABBITMQ_HOST = "localhost"
QUEUE_NAME = "messages"

def send_message_to_queue(message):
    connection = pika.BlockingConnection(
        pika.ConnectionParameters(host=RABBITMQ_HOST, credentials=pika.PlainCredentials('admin', 'admin'))
    )
    channel = connection.channel()
    channel.queue_declare(queue=QUEUE_NAME)
    channel.basic_publish(exchange='', routing_key=QUEUE_NAME, body=message)
    connection.close()

@app.route('/send', methods=['POST'])
def send():
    data = request.get_json()
    message = data.get('message', 'Hello RabbitMQ!')
    
    send_message_to_queue(message)
    
    return jsonify({"status": "Message sent", "message": message})

if __name__ == '__main__':
    app.run(debug=True, port=5000)
