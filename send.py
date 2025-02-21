import pika
from flask import Flask, request, jsonify



app = Flask(__name__)


def send_message(message):
    credentials = pika.PlainCredentials('admin', 'admin')
    connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost', credentials=credentials))
    channel = connection.channel()
    
    channel.queue_declare(queue='hello')
    channel.basic_publish(exchange='', routing_key='hello', body=message)
    
    connection.close()


@app.route('/send', methods=['POST'])
def send():
    data = request.get_json()
    message = data.get('message', 'Hello World!')
    
    send_message(message)
    
    return jsonify({"status": "Message sent", "message": message})

if __name__ == '__main__':
    app.run(debug=True, port=5000)