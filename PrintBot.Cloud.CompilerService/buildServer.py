#!/usr/bin/python3

from subprocess import call
from os import remove
from azure.storage.queue import QueueService
from azure.storage.blob import BlockBlobService
import time

block_blob_service = BlockBlobService(account_name='printbot', account_key='rs3jTKcqgGuAEUPVFLRvq6miUmgH0d9oHqo1Zqd4hxwk3h+5s1ITzY4l1ylinFqqpAoX4fHr/DK8oBUPuDRJGw==')
queue_service = QueueService(account_name='printbot', account_key='rs3jTKcqgGuAEUPVFLRvq6miUmgH0d9oHqo1Zqd4hxwk3h+5s1ITzY4l1ylinFqqpAoX4fHr/DK8oBUPuDRJGw==')


#init pio project
call(['pio', 'init', '-b', '1284p16m', '--project-dir', '/tmp/printBot'])

while True:
        messages = queue_service.get_messages('compilejobs',num_messages=4,visibility_timeout=32)
        for message in messages:
                print("found new job: "+message.content)

                block_blob_service.get_blob_to_path('sourcefiles', message.content, '/tmp/printBot/src/main.cpp')
                print("downloaded file")
                call(['pio', 'run', '--project-dir', '/tmp/printBot'])
                print("start uploading")
                block_blob_service.create_blob_from_path('hexfiles', message.content, '/tmp/printBot/.pioenvs/1284p16m/firmware.hex')
                print("finished uploading")
                queue_service.delete_message('compilejobs', message.id, message.pop_receipt)
                print("deleted message from queue")
                queue_service.put_message('finishedcompilejobs', message.content)
                print("sent new message")

                call(['pio', 'run', '--target', 'clean', '--project-dir', '/tmp/printBot'])
                remove('/tmp/printBot/src/main.cpp')
                print("removed sourcefile")
        time.sleep(2)