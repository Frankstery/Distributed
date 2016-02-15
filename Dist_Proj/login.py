from flask import Flask, render_template, request

app = Flask(__name__)

@app.route("/", methods=['post', 'get'])

def login():
    if request.method=='POST':
        return render_template("welcome.html");
    return render_template("login.html");

app.run(debug=True)