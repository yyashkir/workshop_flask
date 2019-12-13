	General start if virtual environment is already installed
Go to C:\home\projects\workshop_flask
click consequently:
		activate_flask.bat
		run_server.bat
		open_interface
*******************************************************************************

#First time

    virtualenv -p python env
    env\Scripts\activate.bat
    pip install -r requirements.txt
	
#	
SETX FLASK_ENV development
SETX FLASK_APP flaskr
env\Scripts\activate.bat

#open cmd in project root directory
> cd flask_app
> flask run

#pops up in cmd this:
 * Serving Flask app "flaskr" (lazy loading)
 * Environment: development
 * Debug mode: on
 * Restarting with stat
 * Debugger is active!
 * Debugger PIN: 116-074-405
 * Running on http://127.0.0.1:5000/ (Press CTRL+C to quit)
	
#open browser, use address: http://127.0.0.1:5000/app_runner/start
#   @bp.route('/start', methods=['GET', 'POST']) changed to @bp.route('/', methods=['GET', 'POST'])
# browser call is now: http://127.0.0.1:5000/app_runner/

	
#shut down (in new terminal):
> env\Scripts\deactivate.bat


Q/A
- what is taking command "flask run"?
- meaning of "http://127.0.0.1:5000/app_runner/start" ?
- what is "tester.py"
- removed "tester.py" from /flask_app_runner
- flask_app\tester.py
- how to edit web page?

***********************************
@bp.route('/', methods=['GET', 'POST'])
