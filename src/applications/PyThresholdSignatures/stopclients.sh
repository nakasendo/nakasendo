ps -ef | grep client.py | grep -v grep | awk '{print $2}' | xargs kill
