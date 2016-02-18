from flask import Flask, render_template, request

app = Flask(__name__)

# Note the addition of the methods parameter
@app.route("/", methods=['post', 'get'])
def someFunc():
    if request.method=='POST':
		if request.form["submit"] == "Login":
			with open("test.txt", "r") as fo:
				for line in fo:
					user = line.split()
					if user[0] == request.form["Username"] and user[1] == request.form["Password"]: #Checks if username and pass matches
						return render_template("welcome.html")
				return render_template("login.html", error = "Invalid Username or Password")
		elif request.form["submit"] == "Register":
			with open("test.txt","a+") as fo:					#a+ is file descriptor for reading and appending
				for line in fo:
					user = line.split()
					if user[0] == request.form["Username"]:		#Checks if username is already taken
						return render_template("login.html", error = "Invalid Username or Password")				
	   			fo.write(request.form["Username"] + ' ')		#Writes the value of username into the file
				fo.write(request.form["Password"] + ' ' + "\n") #Writers the value of password into file
			return render_template("welcome.html")
    return render_template("login.html")

app.run("127.0.0.1", 13000, debug=True)
