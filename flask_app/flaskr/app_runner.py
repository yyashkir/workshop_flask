import functools
import subprocess
import threading
import os

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for
)

from werkzeug.utils import secure_filename


import uuid
tasks = {}

bp = Blueprint('app_runner', __name__, url_prefix='/app_runner')

tmp_folder = 'tmp'

@bp.route('/start', methods=['GET', 'POST'])
def start():
    if request.method == 'POST':

        task_id = uuid.uuid4().hex
        error = None

        power = request.form['power']
        expansion = request.form['expansion']
        matrix_file = request.files['matrix_file']

        if not power:
            error = 'no power'
        elif not expansion:
            error = 'no expansion'
        elif not matrix_file:
            error = 'no matrix file'

        if error is None:
            filename = secure_filename(matrix_file.filename)
            matrix_save_fname = os.path.join('tmp', filename)
            matrix_file.save(matrix_save_fname)

            output_fname = f'out_{filename}'
            output_path= os.path.join('flaskr/static/files/', output_fname)
            exec_line = f"{os.path.normpath('../matrix_app/matrix_app.exe')} {power} {expansion} {matrix_save_fname} {output_path}"
            print(exec_line)

            proc = subprocess.Popen(exec_line, shell=True, stdout=subprocess.PIPE)
            res = ''
            for line in proc.stdout:
                res = res + str(line)
            tasks[task_id] = { 'result_data' : res, 
                    'output_fname' : output_fname,
                    'output_path' : output_path}
            return redirect(url_for('app_runner.result', task_id=task_id))

        flash(error)

    return render_template('app_runner/start.html')

@bp.route('/result/<task_id>', methods=['GET'])
def result(task_id):
    task = tasks.get(task_id, False)
    if not task:
        error = 'no task data'
        flash(error)
        result_data=None
    else:
        with open(task['output_path'], 'r') as myfile:
            data=myfile.read()
            result_data={ 'download_path' : url_for('static', filename=f"files/{task['output_fname']}"), 'content' : data }

    return render_template('app_runner/result.html', result_data=result_data)
