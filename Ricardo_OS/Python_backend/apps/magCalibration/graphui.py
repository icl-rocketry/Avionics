import matplotlib.pyplot as plt
import matplotlib as mpl


class MagCalGraphsUI():
    def __init__(self):
        
        plt.style.use('dark_background')

        plt.ion()
        self.fig = plt.figure(figsize=[8,8])
        

        self.mxfig = self.fig.add_subplot(2,2,1)
        self.mxfig.grid(True,which='both',color='grey')
        self.mxfig.set_xlabel('Mz Field Strength')
        self.mxfig.set_ylabel('My Field Strength')
    

        self.myfig = self.fig.add_subplot(2,2,2)
        self.myfig.grid(True,which='both',color='grey')
        self.myfig.set_xlabel('My Field Strength')
        self.myfig.set_ylabel('Mz Field Strength')

        self.mzfig = self.fig.add_subplot(2,2,3)
        self.mzfig.grid(True,which='both',color='grey')
        self.mzfig.set_xlabel('Mz Field Strength')
        self.mzfig.set_ylabel('Mx Field Strength')


        self.fullfig = self.fig.add_subplot(2,2,4,projection='3d')
        self.fullfig.grid(True,which='both',color='grey')
        self.fullfig.set_xlabel('Mx Field Strength')
        self.fullfig.set_ylabel('My Field Strength')
        self.fullfig.set_zlabel('Mz Field Strength')
        self.fullfig.xaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
        self.fullfig.yaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
        self.fullfig.zaxis.set_pane_color((1.0, 1.0, 1.0, 0.0))
        

        self.mxfig_rawdata, = self.mxfig.plot([],[],marker='x',ls='',color='red')
        self.myfig_rawdata, = self.myfig.plot([],[],marker='x',ls='',color='blue')
        self.mzfig_rawdata, = self.mzfig.plot([],[],marker='x',ls='',color='green')
        self.fullfig_rawdata, = self.fullfig.plot([],[],[],marker='x',ls='',color='yellow')

        self.mxfig_calibrateddata, = self.mxfig.plot([],[],marker='+',ls='',color='white')
        self.myfig_calibrateddata, = self.myfig.plot([],[],marker='+',ls='',color='white')
        self.mzfig_calibrateddata, = self.mzfig.plot([],[],marker='+',ls='',color='white')
        self.fullfig_calibrateddata, = self.fullfig.plot([],[],[],marker='+',ls='',color='magenta')

        plt.show(block=False)



    def updateMagField(self,magData):
        self.mxfig_rawdata.set_data(magData['mx'],magData['my'])
        self.myfig_rawdata.set_data(magData['my'],magData['mz'])
        self.mzfig_rawdata.set_data(magData['mz'],magData['mx'])
        self.fullfig_rawdata.set_data_3d(magData['mx'],magData['my'],magData['mz'])
        self.rescale_redraw()

    
    def clearPlots(self):
        self.mxfig_rawdata.set_data([],[])
        self.myfig_rawdata.set_data([],[])
        self.mzfig_rawdata.set_data([],[])
        self.fullfig_rawdata.set_data_3d([],[],[])

        self.mxfig_calibrateddata.set_data([],[])
        self.myfig_calibrateddata.set_data([],[])
        self.mzfig_calibrateddata.set_data([],[])
        self.fullfig_calibrateddata.set_data_3d([],[],[])

        self.rescale_redraw()

    def plotCorrectedData(self,correctedMagData):
        self.mxfig_calibrateddata.set_data(correctedMagData['mx'],correctedMagData['my'])
        self.myfig_calibrateddata.set_data(correctedMagData['my'],correctedMagData['mz'])
        self.mzfig_calibrateddata.set_data(correctedMagData['mz'],correctedMagData['mx'])
        self.fullfig_calibrateddata.set_data_3d(correctedMagData['mx'],correctedMagData['my'],correctedMagData['mz'])

        self.rescale_redraw()
        
    def rescale_redraw(self):
        self.mxfig.relim()
        self.mxfig.autoscale_view()
        self.myfig.relim()
        self.myfig.autoscale_view()
        self.mzfig.relim()
        self.mzfig.autoscale_view()
        self.fullfig.relim()
        self.fullfig.autoscale_view(True,True,True)

        self.fig.canvas.draw_idle() # request redraw on next gui event loop update

    def update(self): #drive gui event loop
        plt.pause(.001)
