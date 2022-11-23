# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Jun 17 2015)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc
import wx.richtext

###########################################################################
## Class MainFrame
###########################################################################

class MainFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"PanelShock VCT | Schneider Electric Magelis HMI v1.0.1", pos = wx.DefaultPosition, size = wx.Size( 600,500 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.DefaultSize, wx.DefaultSize )
		self.SetForegroundColour( wx.Colour( 0, 0, 0 ) )
		self.SetBackgroundColour( wx.Colour( 255, 255, 255 ) )
		
		self.m_menubar1 = wx.MenuBar( 0 )
		self.file_menu = wx.Menu()
		self.exit_menuItem = wx.MenuItem( self.file_menu, wx.ID_ANY, u"Exit", wx.EmptyString, wx.ITEM_NORMAL )
		self.file_menu.AppendItem( self.exit_menuItem )
		
		self.m_menubar1.Append( self.file_menu, u"File" ) 
		
		self.help_menu = wx.Menu()
		self.updates_menuItem = wx.MenuItem( self.help_menu, wx.ID_ANY, u"Updates", wx.EmptyString, wx.ITEM_NORMAL )
		self.help_menu.AppendItem( self.updates_menuItem )
		
		self.help_menu.AppendSeparator()
		
		self.about_menuItem = wx.MenuItem( self.help_menu, wx.ID_ANY, u"About", wx.EmptyString, wx.ITEM_NORMAL )
		self.help_menu.AppendItem( self.about_menuItem )
		
		self.m_menubar1.Append( self.help_menu, u"Help" ) 
		
		self.SetMenuBar( self.m_menubar1 )
		
		main_bSizer = wx.BoxSizer( wx.VERTICAL )
		
		self.m_bitmap1 = wx.StaticBitmap( self, wx.ID_ANY, wx.Bitmap( u"images/banner.jpg", wx.BITMAP_TYPE_ANY ), wx.DefaultPosition, wx.DefaultSize, 0 )
		main_bSizer.Add( self.m_bitmap1, 0, wx.ALL|wx.EXPAND, 5 )
		
		fgSizer1 = wx.FlexGridSizer( 0, 4, 0, 0 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		vuln_type_sbSizer = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Vulnerability Type" ), wx.VERTICAL )
		
		self.dos_attack = wx.RadioButton( vuln_type_sbSizer.GetStaticBox(), wx.ID_ANY, u"Denial of Service (DoS)", wx.DefaultPosition, wx.DefaultSize, 0 )
		vuln_type_sbSizer.Add( self.dos_attack, 0, wx.ALL, 5 )
		
		self.pf_attack = wx.RadioButton( vuln_type_sbSizer.GetStaticBox(), wx.ID_ANY, u"PanelShock", wx.DefaultPosition, wx.DefaultSize, 0 )
		vuln_type_sbSizer.Add( self.pf_attack, 0, wx.ALL, 5 )
		
		
		fgSizer1.Add( vuln_type_sbSizer, 1, wx.EXPAND|wx.ALL, 5 )
		
		ip_addr_sbSizer = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Target IP Address" ), wx.VERTICAL )
		
		self.ip_addr = wx.TextCtrl( ip_addr_sbSizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( 180,-1 ), 0 )
		ip_addr_sbSizer.Add( self.ip_addr, 0, wx.ALL, 5 )
		
		
		fgSizer1.Add( ip_addr_sbSizer, 1, wx.EXPAND|wx.ALL, 5 )
		
		port_sbSizer = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Target Port" ), wx.VERTICAL )
		
		self.port = wx.TextCtrl( port_sbSizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.Size( 60,-1 ), 0 )
		port_sbSizer.Add( self.port, 0, wx.ALL, 5 )
		
		
		fgSizer1.Add( port_sbSizer, 1, wx.EXPAND|wx.ALL, 5 )
		
		self.check = wx.Button( self, wx.ID_ANY, u"Check", wx.DefaultPosition, wx.DefaultSize, 0 )
		fgSizer1.Add( self.check, 0, wx.ALL|wx.ALIGN_CENTER_VERTICAL, 5 )
		
		
		main_bSizer.Add( fgSizer1, 1, wx.EXPAND, 5 )
		
		console_sbSizer = wx.StaticBoxSizer( wx.StaticBox( self, wx.ID_ANY, u"Console" ), wx.VERTICAL )
		
		console_sbSizer.SetMinSize( wx.Size( -1,200 ) ) 
		self.console = wx.richtext.RichTextCtrl( console_sbSizer.GetStaticBox(), wx.ID_ANY, wx.EmptyString, wx.DefaultPosition, wx.DefaultSize, wx.TE_READONLY|wx.VSCROLL|wx.HSCROLL|wx.NO_BORDER|wx.WANTS_CHARS )
		console_sbSizer.Add( self.console, 1, wx.EXPAND |wx.ALL, 5 )
		
		
		main_bSizer.Add( console_sbSizer, 1, wx.ALL|wx.EXPAND, 5 )
		
		self.m_staticText1 = wx.StaticText( self, wx.ID_ANY, u"Powered by SCADAGate+ Analyzer by CRITIFENCE | http://www.critifence.com", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText1.Wrap( -1 )
		main_bSizer.Add( self.m_staticText1, 0, wx.ALL, 5 )
		
		
		self.SetSizer( main_bSizer )
		self.Layout()
		self.sb = self.CreateStatusBar( 1, wx.ST_SIZEGRIP, wx.ID_ANY )
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_MENU, self.exitFunc, id = self.exit_menuItem.GetId() )
		self.Bind( wx.EVT_MENU, self.updateFunc, id = self.updates_menuItem.GetId() )
		self.Bind( wx.EVT_MENU, self.aboutFunc, id = self.about_menuItem.GetId() )
		self.dos_attack.Bind( wx.EVT_RADIOBUTTON, self.dos_attack_func )
		self.pf_attack.Bind( wx.EVT_RADIOBUTTON, self.pf_attack_func )
		self.check.Bind( wx.EVT_BUTTON, self.start_check )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def exitFunc( self, event ):
		event.Skip()
	
	def updateFunc( self, event ):
		event.Skip()
	
	def aboutFunc( self, event ):
		event.Skip()
	
	def dos_attack_func( self, event ):
		event.Skip()
	
	def pf_attack_func( self, event ):
		event.Skip()
	
	def start_check( self, event ):
		event.Skip()
	

