# Setup

      virtualenv -p python3 env
      . env/bin/activate.fish #for fish
      source env/bin/activate #for bash
      pip install -r requirements.txt

## Start server

    export FLASK_ENV=development
    export FLASK_APP=flaskr
    
    flask run


## See browser

    (http://localhost:5000)[http://localhost:5000]

http://127.0.0.1:5000/