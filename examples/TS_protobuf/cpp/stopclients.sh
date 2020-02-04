ps -ef | grep tsClient | grep -v grep | awk '{print $2}' | xargs kill
