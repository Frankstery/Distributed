from flask import Flask, session, redirect, url_for, escape, request, render_template

app = Flask(__name__)

def login(username, password, fileOfUsers):
	with open(fileOfUsers, "r") as fo:
		for line in fo:
			user = line.split()
			if user[0] == username and user[1] == password: 	#Checks if username and pass matches
				session['username'] = username
				return redirect("/user")
	return render_template("login.html", error = "Invalid Username or Password")


def register(username, password, fileOfUsers):
	with open(fileOfUsers,"a+") as fo:							#a+ is file descriptor for reading and appending
		for line in fo:
			user = line.split()
			if user[0] == username:								#Checks if username is already taken
				return render_template("login.html", error = "Username already taken")				
		fo.write(username + ' ')								#Writes the value of username into the file
		fo.write(password + ' ' + "\n") 						#Writers the value of password into file
	open(username + 'Msgs' + '.txt', 'w')
	open(username + 'Friends' + '.txt', 'w')
	return render_template("welcome.html")


def add_friend(friend, fileOfUsers, fileOfFriends):
    with open(fileOfUsers, "r") as fo:
        for line in fo:
            user = line.split()
            if user[0] == friend:
                with open(fileOfFriends, 'a') as fof:
                    fof.write(username + '\n')
                return render_template("user.html")
            else:
                return render_template("user.html", aferror = "No user by that name")



@app.route("/", methods=['post', 'get'])
def home():
    if request.method=='POST':
		if request.form["submit"] == "Login":
			return login(request.form["Username"], request.form["Password"], "listOfUsers.txt")			
		elif request.form["submit"] == "Register":
			return register(request.form["Username"], request.form["Password"], "listOfUsers.txt")
    return render_template("login.html")


@app.route('/logout')
def logout():
    # remove the username from the session if it's there
    session.pop('username'.txt, None)
    return redirect(url_for('index'))


@app.route("/user", methods=['post', 'get'])
def loadUserData():
	username = session['username']											#Gets the username of the person
	listOfMsgs = []															
	with open(username + 'Msgs' + '.txt', 'a+') as fo:
		for line in fo:
			listOfMsgs.append(line)
	render_template("user.html", user = username, messages = listOfMsgs)
	if request.method=='POST':
		with open(username + 'Msgs' + '.txt', 'a+') as fo:
			fo.write(request.form["tweet"] + ' ' + "\n")					#Gets the values inside the textbox and adds to file
			return redirect("/user")
	return render_template("user.html", user = username, messages = listOfMsgs)
	


app.secret_key = "rj;elkjxia"
app.run("127.0.0.1", 13000, debug=True)
