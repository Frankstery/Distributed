from flask import Flask, session, redirect, url_for, escape, request, render_template
import os, socket
app = Flask(__name__)


def send(lineOfText):
	s = socket.socket()         # Create a socket object
	host = "127.0.0.1" # Where do you want to connect
	port = 13002                # port to connect to
	s.connect((host, port))
	s.send(lineOfText.encode())
	returnMsg =s.recv(1024)
	return returnMsg
	s.close                     # Close the socket when done

def sendForMsg(lineOfText):
	s = socket.socket()         # Create a socket object
	host = "127.0.0.1" # Where do you want to connect
	port = 13002                # port to connect to
	s.connect((host, port))
	s.send(lineOfText.encode())
	fullMsgs = [];
	while True:
		if returnMsg == "End of Messages":
			break;
		returnMsg =s.recv(1024);
		fullMsgs.append(returnMsg);
	return fullMsgs
	s.close    



@app.route("/", methods=['post', 'get'])
def home():
    if request.method=='POST':
        if request.form["submit"] == "Login":
            str = "LOGIN" + ' ' + request.form["Username"] + ' ' + request.form["Password"] + " \n"
            returnMsg = send(str)
            if returnMsg == "LOGIN":
                session["username"] = request.form["Username"]
                return redirect("/user")
            else:
                return render_template("login.html", error = "Invalid Username or Password")   
        elif request.form["submit"] == "Register":
            str = "REGISTER" + ' ' + request.form["Username"] + ' ' + request.form["Password"] + " \n"
            returnMsg = send(str)
            if returnMsg == "REGISTERED":
				return redirect(url_for("welcomeToUser", loginName = request.form["Username"]))
            else:
                return render_template("login.html", error = "Username already taken")         
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
	if 'username' not in session:
		return redirect(url_for('home'))
	username = session['username']											#Gets the username of the person
	listOfMsgs = []
	listofFriends = []
	str = "GETFRIENDS " + username
	returnMsg = send(str)
	listOfFriends = returnMsg.split()
	# with open(username + 'Friends' + '.txt', 'a+') as fo:
	# 	for line in fo:
	# 		listOfFriends.append(line)
																	
	# with open(username + 'Msgs' + '.txt', 'a+') as fo:						#Loads all the data into two arrays
	# 		for line in fo:
	# 			listOfMsgs.append(line)  
	if request.method=='POST':
		if request.form["submit"] == "Post":
			str = "ADDMSG" + ' ' + username + ' ' + request.form["tweet"];
			print send(str)
			# with open(username + 'Msgs' + '.txt', 'a+') as fo:
			# 	fo.write(request.form["tweet"] + ' ' + "\n")					#Gets the values inside the textbox and adds to file
			# 	return redirect("/user")
		elif request.form["submit"] == "Add":
			#           errormsg = add_friend(request.form["Username"], username + 'Friends' + '.txt')
			#           return redirect("/user")
			str = "ADDFRIEND " + username + " " + request.form["Username"] + " " + username + "Friends" + ".txt"
			errormsg = send(str)
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
			str = "DELETE " + session['username']
			send(str)
			#delete_account(session['username'])
			return redirect(url_for('home'))
	return render_template("removeAccount.html")
	


@app.route('/logout')
def logout():
    # remove the username from the session if it's there
    session.pop('username', None)
    #print session[0]
    return redirect(url_for('home'))
    


app.secret_key = "rj;elkjxia"
app.run("127.0.0.1", 13000, debug=True)

