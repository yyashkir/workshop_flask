import os

from flask import Flask
import subprocess

def create_app(test_config=None):
    # create and configure the app
    app = Flask(__name__, instance_relative_config=True)
    app.config.from_mapping(
        SECRET_KEY='dev',
        DATABASE=os.path.join(app.instance_path, 'flaskr.sqlite'),
    )

    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
    else:
        # load the test config if passed in
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    # a simple page that says hello
    @app.route('/hello')
    def hello():
        proc = subprocess.Popen("python tester.py", bufsize=1, universal_newlines=True, shell=True, stdout=subprocess.PIPE)
        print('Proc open')
        print(proc.stdout.readline())
        #from_shell = str(proc.stdout.readline())
        return 'Hello, World! From outside' #+ from_shell

    from . import app_runner 
    app.register_blueprint(app_runner.bp)

    return app

    
