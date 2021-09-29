import cmd2
from cmd2.argparse_custom import Cmd2ArgumentParser
from cmd2.decorators import with_argparser

class MagCalCmdUI(cmd2.Cmd):
    def __init__(self,msgQ):
        super().__init__(allow_cli_args=False)
        self.msgQ = msgQ
    
    def do_stopdata(self,line):
        self.msgQ.put({'msg':"STOPDATA"})

    
    def do_startdata(self,line):
        self.msgQ.put({"msg":"STARTDATA"})
 
    
    def do_cleardata(self,line):
        self.msgQ.put({'msg':"CLEARDATA"})

    calculate_ap = Cmd2ArgumentParser()
    calculate_ap.add_argument("-v",'--verbose',required=False,action='store_true',default=False,help='toggle verbose calculation')
    
    @with_argparser(calculate_ap)
    def do_calculate(self,opts):
        self.msgQ.put({'msg':'CALCULATE',
                        'verbose':opts.verbose})
    
    sendcalibration_ap = Cmd2ArgumentParser()
    sendcalibration_ap.add_argument("-d",'--destination',required=False,type=int,help='set destination address',default=2)
    sendcalibration_ap.add_argument('-s','--source',required=False,type=int,help='set source address',default=4)
    sendcalibration_ap.add_argument('--fieldMagnitude',required=False,type=float,help='set mag field magnitude',default=1)
    sendcalibration_ap.add_argument('--inclination',required=False,type=float,help='set mag inclination',default=0)
    sendcalibration_ap.add_argument('--declination',required=False,type=float,help='set mag declination',default=0)
    
    @with_argparser(sendcalibration_ap)
    def do_sendcalibration(self,opts):
        self.msgQ.put({'msg':"SENDCALIBRATION",
                       'destination':opts.destination,
                       'source':opts.source,
                       'fieldMagnitude':opts.fieldMagnitude,
                       'inclination':opts.inclination,
                       'declination':opts.declination})
    
    def do_printcal(self,line):
        self.msgQ.put({'msg':"PRINTCAL"})
    
    def do_visualize(self,line):
        self.msgQ.put({'msg':"VISUALIZE"})
    
    loadfile_ap = Cmd2ArgumentParser()
    loadfile_ap.add_argument("-f",'--filename',required=True,type=str,help='data file path')
    @with_argparser(loadfile_ap)
    def do_loadfile(self,opts):
        self.msgQ.put({'msg':'LOADFILE',
                       'filename':opts.filename})

    savefile_ap = Cmd2ArgumentParser()
    savefile_ap.add_argument("-f",'--filename',required=True,type=str,help='save file name')
    @with_argparser(savefile_ap)
    def do_savefile(self,opts):
        self.msgQ.put({'msg':'SAVEFILE',
                       'filename':opts.filename})

    def sigint_handler(self):
        return True
        
