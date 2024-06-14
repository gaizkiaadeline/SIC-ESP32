
from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.json
    temperature = data.get('temperature')
    humidity = data.get('humidity')

    # Perform operations according to received data
    # Example: Store data in database, perform analysis, etc.

    response = {'message': 'Data received successfully',
                'temperature': temperature,
                'humidity': humidity}
    return jsonify(response)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=True)
    