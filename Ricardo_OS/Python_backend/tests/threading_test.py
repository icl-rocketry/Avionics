import threading
def get_threads(location=''):
    print(location)
    print("threading.current_thread()")
    print(threading.current_thread())
    print(id(threading.current_thread()))
    print("threading.main_thread()")
    print(threading.main_thread())
    print(id(threading.main_thread()))

get_threads("before eventlet")
import eventlet
get_threads("after eventlet")
eventlet.monkey_patch()
get_threads("after monkey patch")

print(vars(threading.main_thread()))
print(vars(threading.current_thread()))

