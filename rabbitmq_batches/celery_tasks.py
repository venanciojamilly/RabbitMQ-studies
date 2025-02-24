from celery.schedules import crontab
from worker import app, process_batch

app.conf.beat_schedule = {
    'process_batch_every_10_seconds': {
        'task': 'worker.process_batch',
        'schedule': 10.0,  # A cada 10 segundos
    },
}
