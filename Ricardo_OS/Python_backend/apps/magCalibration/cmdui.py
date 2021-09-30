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
    
    loaddata_ap = Cmd2ArgumentParser()
    loaddata_ap.add_argument("-f",'--filename',required=False,type=str,help='data file path',default='magdata.csv')
    @with_argparser(loaddata_ap)
    def do_loaddata(self,opts):
        self.msgQ.put({'msg':'LOADDATA',
                       'filename':opts.filename})

    savedata_ap = Cmd2ArgumentParser()
    savedata_ap.add_argument("-f",'--filename',required=False,type=str,help='save file name',default='magdata.csv')
    @with_argparser(savedata_ap)
    def do_savedata(self,opts):
        self.msgQ.put({'msg':'SAVEDATA',
                       'filename':opts.filename})
    
    savecal_ap = Cmd2ArgumentParser()
    savecal_ap.add_argument("-f",'--filename',required=False,type=str,help='save file name',default='magcal.txt')
    @with_argparser(savecal_ap)
    def do_savecal(self,opts):
        self.msgQ.put({'msg':'SAVECAL',
                       'filename':opts.filename})


    def sigint_handler(self):
        return True
        
