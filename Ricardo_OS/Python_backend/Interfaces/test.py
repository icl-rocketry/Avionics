import urwid
from datetime import datetime

class LoadingView():
    def __init__(self):
        
        self.logotext = [('logo_blue',u"██╗ ██████╗██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")]
        self.logo = urwid.Text(self.logotext)
        #stores the top widget
        self.top = urwid.Filler(self.logo)
    def changeText(self,main,data):
        #self.logo.set_text([('logo_blue',u"██╗ ██████╗"),('logo_white',u"██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝"),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗"),('logo_white',u"███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝"),('logo_white',u"╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")])
        self.logotext = [('logo_blue',u"██╗ ██████╗"),('logo_white',u"██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝"),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗"),('logo_white',u"███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝"),('logo_white',u"╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")]
    
class Logo(urwid.WidgetWrap):
    def __init__(self):
        self.logotext = [('logo_blue',u"██╗ ██████╗██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")]
        self.logo = urwid.Text(self.logotext)
        #stores the top widget
        self.top = urwid.Filler(self.logo)
        super().__init__(self.top)
    
    def changeText(self,main,data):
        
        #self.logo.set_text([('logo_blue',u"██╗ ██████╗"),('logo_white',u"██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝"),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗"),('logo_white',u"███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝"),('logo_white',u"╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")])
        #self.logotext = [('logo_blue',u"██╗ ██████╗"),('logo_white',u"██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝"),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗"),('logo_white',u"███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝"),('logo_white',u"╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")]
        self.logo.set_text([('status_bar',datetime.now().strftime("%H:%M:%S"))])

logotext = [('logo_blue',u"██╗ ██████╗██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     ██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")]
logo = urwid.Text(logotext)
top = urwid.Filler(logo)
palette = {
            ('bg','light gray','black','','#0b1518','#dbdbdb'),
            ('status_bar','light gray','black','','#0b1518','#dbdbdb'),
            ('error','dark red','black','blink','#ff0000','#dbdbdb'),
            ('nominal','light green','black','','#20ff00','#dbdbdb'),
            ('logo_blue','dark blue','black','','#001240','#dbdbdb'),
            ('logo_red','light red','black','','#cf0000','#dbdbdb'),
            ('logo_white','white','black','','#ffffff','#dbdbdb')
        }
loadingview :LoadingView = LoadingView()
logo = Logo()
loop :urwid.MainLoop = urwid.MainLoop(logo,palette)

def changeText(main,data):
    #original_widget = main.widget.original_widget
    
    logo.set_text([('logo_blue',u"██╗ ██████╗"),('logo_white',u"██╗     "),('logo_red',u"██████╗\n"),('logo_blue',u"██║██╔════╝"),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██████╔╝\n"),('logo_blue',u"██║██║     "),('logo_white',u"██║     "),('logo_red',u"██╔══██╗\n"),('logo_blue',u"██║╚██████╗"),('logo_white',u"███████╗"),('logo_red',u"██║  ██║\n"),('logo_blue',u"╚═╝ ╚═════╝"),('logo_white',u"╚══════╝"),('logo_red',u"╚═╝  ╚═╝\n")])
    
loop.set_alarm_in(3,logo.changeText)
loop.run()
