from flask import Flask, request, render_template, jsonify

app = Flask(__name__)

device_data = {"wifi": 0, "bluetooth": 0}

@app.route('/')
def index():
    return render_template('index.html', wifi=device_data["wifi"], bluetooth=device_data["bluetooth"])

@app.route('/update', methods=['POST'])
def update_data():
    global device_data
    data = request.json
    print("Received data:", data)  # Debugging output
    if "wifi" in data and "bluetooth" in data:
        device_data["wifi"] = data["wifi"]
        device_data["bluetooth"] = data["bluetooth"]
    return jsonify({"status": "success"}), 200

@app.route('/data')
def get_data():
    return jsonify(device_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)  # Allows all network devices to connect