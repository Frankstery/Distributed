from flask import Flask, session, redirect, url_for, escape, request, render_template
import os
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
	open(username + 'Msgs' + '.txt', 'w')						#Creates the files
	with open(username + 'Friends' + '.txt', 'w') as start:
		start.write("Home" + ' ' + '\n')
	return redirect(url_for("welcomeToUser", loginName = username))



def add_friend(friend, fileOfFriends):
	with open("listOfUsers.txt", "r") as fo:
		for line in fo:
			user = line.split()
			if user[0] == friend:
				with open(fileOfFriends, 'a+') as fof:
					for line in fof:
						if line == friend:
							return "Friend already added"
						elif friend == session['username']:
							return "You can't add yourself"
 					fof.write(friend + '\n')
					return
	return "No such User"				

def deleteLineFromFile(fileName, user):
	f = open(fileName,"r")
	lines = f.readlines()
	f.close()
	f = open(fileName,"w")
	for line in lines:
		username = line.split()
  		if username[0] != user:
			f.write(line)
	f.close()


def delete_account(user):
	deleteLineFromFile("listOfUsers.txt", user)
	for i in os.listdir(os.getcwd()):
		if i == user + 'Friends' + '.txt' or i == user + 'Msgs' + '.txt':
			os.remove(i)
		if i.endswith("Friends.txt") and user not in i: 
			for line in i:
				deleteLineFromFile(i, user)



@app.route("/", methods=['post', 'get'])
def home():
    if request.method=='POST':
		if request.form["submit"] == "Login":
			return login(request.form["Username"], request.form["Password"], "listOfUsers.txt")			
		elif request.form["submit"] == "Register":
			return register(request.form["Username"], request.form["Password"], "listOfUsers.txt")
    return render_template("login.html")


@app.route("/welcome", methods=['post', 'get'])
def welcomeToUser():
	username = request.args['loginName']
	if request.method=='POST':
		session['username'] = username
		return redirect("/user")
	return render_template("welcome.html")




@app.route("/user", methods=['POST', 'GET'])
def loadUserData():

	username = session['username']											#Gets the username of the person
	listOfMsgs = []
	listOfFriends = []	
	
	with open(username + 'Friends' + '.txt', 'a+') as fo:
		for line in fo:
			listOfFriends.append(line)
																	
	with open(username + 'Msgs' + '.txt', 'a+') as fo:						#Loads all the data into two arrays
			for line in fo:
				listOfMsgs.append(line)  

	if request.method=='POST':
		if request.form["submit"] == "Post":
			with open(username + 'Msgs' + '.txt', 'a+') as fo:
				fo.write(request.form["tweet"] + ' ' + "\n")					#Gets the values inside the textbox and adds to file
				return redirect("/user")
		elif request.form["submit"] == "Add":
				errormsg = add_friend(request.form["Username"], username + 'Friends' + '.txt')
				return redirect("/user")			
		elif request.form["submit"] == "Logout":
			return redirect("/logout")
		elif request.form["submit"] == "Delete Account":
			return redirect("/delete")
		else:
			newActive = request.form["submit"]
			if newActive == "Home":
				return redirect("/user")
			return redirect(url_for('loadFriendData', friend = newActive))
	return render_template("user.html", user = username, messages = listOfMsgs, friends = listOfFriends, displayedPage = "Home")


@app.route('/friend/<friend>', methods = ["POST", "GET"])
def loadFriendData(friend):
	username = session['username']											#Gets the username of the person
	listOfMsgs = []
	listOfFriends = []	
	
	with open(username + 'Friends' + '.txt', 'a+') as fo:
		for line in fo:
			listOfFriends.append(line)
																	
	with open(friend + 'Msgs' + '.txt', 'a+') as fo:						#Loads all the data into two arrays
			for line in fo:
				listOfMsgs.append(line)

	if request.method=='POST':
		if request.form["submit"] == "Add":
				errormsg = add_friend(request.form["Username"], username + 'Friends' + '.txt')
				return redirect(url_for('loadFriendData', friend = friend))			
		elif request.form["submit"] == "Logout":
			return redirect("/logout")

		else:
			newActive = request.form["submit"]
			if newActive == "Home":
				return redirect("/user")
			return redirect(url_for('loadFriendData', friend = newActive))
	return render_template("friend.html", user = username, messages = listOfMsgs, friends = listOfFriends, displayedPage = friend)



@app.route('/delete', methods=["POST", "GET"])
def delete():
	if request.method=='POST':
		if request.form["submit"] == "Back":
			return redirect("/user")
		elif request.form["submit"] == "Confirm Delete":
			delete_account(session['username'])
			return redirect(url_for('home'))
	return render_template("removeAccount.html")
	


@app.route('/logout')
def logout():
    # remove the username from the session if it's there
    session.pop(session['username'], None)
    return redirect(url_for('home'))


app.secret_key = "rj;elkjxia"
app.run("127.0.0.1", 13000, debug=True)
