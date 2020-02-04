ps -ef | grep client_twisted.py | grep -v grep | awk '{print $2}' | xargs kill
