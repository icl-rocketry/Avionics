import urwid
def urwid_test():
    """
            'black', 'dark red', 'dark green', 'brown', 'dark blue',
            'dark magenta', 'dark cyan', 'light gray', 'dark gray',
            'light red', 'light green', 'yellow', 'light blue', 
            'light magenta', 'light cyan', 'white'
    """

    class MyListBox(urwid.ListBox):
        def focus_next(self):
            try: 
                self.body.set_focus(self.body.get_next(self.body.get_focus()[1])[1])
            except:
                pass
        def focus_previous(self):
            try: 
                self.body.set_focus(self.body.get_prev(self.body.get_focus()[1])[1])
            except:
                pass            

    def handle_input(input):
        if input == "esc":
            raise urwid.ExitMainLoop()
        head.original_widget.set_text("key pressed: %s" % input)
        if input == "up":
            listbox.focus_previous()
        elif input == "down":
            listbox.focus_next()
    palette = [("top","white","black"),
               ("line","light green","dark green","standout"),
               ("frame","dark magenta","white"),
               ]
    widgets = [urwid.AttrMap(widget,None,"line") for widget in
                [
                    urwid.Text("Chemma!"),
                    urwid.Divider("-"),
                    urwid.Text("Another text widget!"),
                    urwid.Divider("-"),                   
                    urwid.Text("What is your name"),
                    urwid.Divider("-"),                   
                    urwid.Text("Boy ?"),                                                            
                ]
              ]
    head    = urwid.AttrMap(urwid.Text("key pressed :",wrap="clip"),"top")
    L       = urwid.SimpleListWalker(widgets)
    listbox = MyListBox(L)
    top     = urwid.AttrMap(urwid.Frame(listbox,head),"frame")
    loop    = urwid.MainLoop(top,palette,unhandled_input=handle_input)
    loop.screen.set_terminal_properties(colors=256)
    loop.run()

if __name__ == "__main__":
    urwid_test()