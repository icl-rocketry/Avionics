import cmd2
from cmd2.argparse_custom import Cmd2ArgumentParser
from cmd2.decorators import with_argparser
import redis
import json
import sys





class CommandLineInterface(cmd2.Cmd):
    def __init__(self,redishost = 'localhost',redisport = 6379):
        super().__init__(allow_cli_args=False)
        self.r : redis.Redis = redis.Redis(host=redishost,port=redisport)
        
    
    task_ap = Cmd2ArgumentParser()
    task_ap.add_argument("-t",'--task',required=True,type=str,help='Task ID')
    task_ap.add_argument('--start',required=False,action='store_true',help='Start Telemetry',default=False)
    task_ap.add_argument('--stop',required=False,action='store_true',help='Stop Telemetry',default=False)
    task_ap.add_argument('--source',required=False,type=int,help='Source Node')
    task_ap.add_argument('--destination',required=False,type=int,help='Destination Node')
    task_ap.add_argument('--dt',required=False,type=int,help='Change Task Time Delta in us')
    
    @with_argparser(task_ap)
    def do_task(self,opts):
        prev_state = self.r.get(opts.task + ":STATE")
        if prev_state is None:
            self.poutput("ERROR -> cant find task id!")
        try:
            new_state = json.loads(prev_state)
        except:
            self.poutput("ERROR -> cannot deserialize json key")
        if opts.start and opts.stop:
            #throw error
            self.poutput("ERROR -> start and stop both specifed")
            return
        #update state
        if opts.start:
            new_state["run"] = True
        if opts.stop:
            new_state["run"] = False
        if opts.source is not None:
            new_state["source"] = opts.source
        if opts.destination is not None:
            new_state["destination"] = opts.destination
        if opts.dt is not None:
            new_state["dt"] = opts.dt
        #push new state to redis
        self.r.set(opts.task + ":STATE",json.dumps(new_state))
    
    def sigint_handler(self, signum, param2) -> None:
        #override default behaviour to exit on ctrl c
        sys.exit(0)
        return





if __name__ == '__main__':
    c = CommandLineInterface()
    c.cmdloop() #start cmd event loop


