import functools
import subprocess
import threading

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for
)


import uuid
tasks = {}

bp = Blueprint('app_runner', __name__, url_prefix='/app_runner')

@bp.route('/start', methods=['GET', 'POST'])
def start():
    if request.method == 'POST':
        x1 = request.form['x1']
        x2 = request.form['x2']

        task_id = uuid.uuid4().hex
        error = None

        if not x1:
            error = 'no x1'
        elif not x2:
            error = 'no x2'

        if error is None:
            proc = subprocess.Popen(f"python tester.py --x1={x1} --x2={x2}", shell=True, stdout=subprocess.PIPE)
            res = ''
            for line in proc.stdout:
                res = res + str(line)
            tasks[task_id] = { 'result_data' : res }
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
        result_data=task['result_data']

    return render_template('app_runner/result.html', result_data=result_data)
