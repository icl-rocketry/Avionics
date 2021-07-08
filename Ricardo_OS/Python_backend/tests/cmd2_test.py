import cmd2
from cmd2.argparse_custom import Cmd2ArgumentParser
from cmd2.decorators import with_argparser

class CmdInterface(cmd2.Cmd):
    def __init__(self):
        super().__init__()
    
    ap = Cmd2ArgumentParser()
    ap.add_argument('-p','--test', type=str)

    @with_argparser(ap)
    def do_echo(self,opts):
        self.poutput(opts.test)
        

if __name__ == '__main__':
    import sys
    c = CmdInterface()
    c.cmdloop()
    