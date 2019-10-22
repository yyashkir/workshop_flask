cd flask_app

virtualenv -p python env

pip install -r requirements.txt

SETX FLASK_ENV development

SETX FLASK_APP flaskr

cd env\Scripts

activate.bat

cd ..\..

flask run

