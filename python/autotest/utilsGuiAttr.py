
# TODO - change first part of assoc array to UPPER case to
# distinguish between 'real' names and 'defines'
# also enforce some naming convention to distinguish between
# a name and an xpath (or id, tag etc)
#
# example:
#   "APPLY_UG_N" : "submit"
#
#   "DIS_AUTO_X" : "(//input[@name='radio_onoff_autoactivate'])[2]"

class SailorGui:
    
    def makeURL(text):
      return text

    pages = {   "dashboard":makeURL("Home"),

                "connect":makeURL("Connect"),

                "phonebook":makeURL("Phone%20book"),
                    "mob_num":makeURL("Mobile%20numbers"),

                "messages":makeURL("Messages"),
                    "inbox":makeURL("Inbox"),
                    "write_msg":makeURL("Write%20message"),
                    "otbx":makeURL("Outbox"),
                    "sent":makeURL("Sent"),
                    "msg_set":makeURL("Message%20settings"),

                "calls":makeURL("Calls"),
                    "out_cal":makeURL("Outgoing%20calls"),
                    "rec_cal":makeURL("Received%20calls"),
                    "mis_cal":makeURL("Missed%20calls"),
                    "std_ses":makeURL("Standard%20data%20sessions"),
                    "str_ses":makeURL("Streaming%20data%20sessions"),

                "settings":makeURL("Settings"),
                    "lan":makeURL("LAN"),
                        "port_fwd":makeURL("Port%20forwarding"),
                        "ntw_dev":makeURL("Network%20devices"),
                        "ntw_cls":makeURL("Network%20classification"),
                        "ntw_ug":makeURL("Network%20user%20groups"),
                        "ntw_ug_mjc":makeURL("Network%20user%20groups&oper=edit&idx=10"),
                        "PPPoE":makeURL("PPPoE"),
                        "stat_rut":makeURL("PPPoE"),

                    "phfx":makeURL("Phone/Fax"),
                    "isdn":makeURL("ISDN"),

                    "common":makeURL("Common"),
                        "call_fwd":makeURL("Call%20forward"),
                        "call_bar":makeURL("Call%20barring"),
                        "call_wt":makeURL("Call%20waiting"),
                        "line_id":makeURL("Line%20identification"),
                        "cls_ug":makeURL("Closed%20user%20group"),
                        "lcl_exg":makeURL("Local%20exchange"),

                    "ip_hdset":makeURL("IP%20Handsets"),
                        "call_set":makeURL("Call%20Settings"),
                        "serv_set":makeURL("Server%20Settings"),

                    "disc_io":makeURL("Discrete%20IO"),
                    "trck":makeURL("Tracking"),
                    "ul_sw":makeURL("Upload"),
                    "sat_sel":makeURL("Satellite%20selection"),
                    "lang":makeURL("Language"),

                    "administration":makeURL("Administration"),
                        "call_chg":makeURL("Call%20charges"),
                        "log_hand":makeURL("Log%20handling"),
                        "data_lmt":makeURL("Data%20limits"),
                        "prfl":makeURL("Profiles"),
                        "trf_flowf":makeURL("Traffic%20flow%20filters"),
                        "sim_pin":makeURL("SIM%20PIN"),
                        "sim_lk":makeURL("SIM%20Lock"),
                        "uperm":makeURL("User%20Permissions"),
                        "rmt_man":makeURL("Remote%20Management"),
                        "lnk_mon":makeURL("Link%20Monitoring"),
                        "rmt_act":makeURL("Remote%20Activation"),
                        "rstr_dlg":makeURL("Restricted%20Dialing"),
                        "mlty_voice":makeURL("Multi-voice"),
                        "voice_dis":makeURL("Voice%20Distress"),

                    "helpdesk":makeURL("Helpdesk"),
                        "evnt_lst":makeURL("Event%20list"),
                        "evnt_log":makeURL("Event%20log"),
                        "ext_stat":makeURL("Extended%20Status"),
                        "slf_test":makeURL("Self%20Test"),
                }

##
##    ## dashboard
    elements = {
                        ## dashboard
                        "start_std":"//a[contains(text(),'Start Standard')]",## start std xpath link text
                        "start_con":"//tr[2]/td/p/a",## start standard Xpath position

                        ## DHCP address given from std connection
                        "dIP_addr":"//tr[13]/td/p",
                        # vh "str_addr":"//tr[14]/td/p",
                        "str_addr":"//tr[15]/td/p",
                        "std_ip2":"//tr[14]/td/p",
                        "stop_std":"//a[contains(text(),'Stop Standard')]",## stop std connection
                        "admin_off_text":"//tr[10]/th/p",## admin log off text
                        "submit_admin":"input[type=\"submit\"]", ##css apply admin login

                        "dIP":"//tr[13]/td/p",## DHCP Ip addr

                        "IP":"//tr[9]/td[3]/p",##console IP

                        "dIP_add2":"//tr[13]/td/p",

                        ## Connect link

                        "constart_std":"(//a[contains(text(),'Start Standard')])[2]",## start std expath link text
                        "con16s":"//a[contains(text(),'Start Streaming 16')]",
                        "con32s":"//a[contains(text(),'Start Streaming 32')]",
                        "con64s":"//a[contains(text(),'Start Streaming 64')]",
                        "con128s":"//a[contains(text(),'Start Streaming 128')]",

                        "constp_std":"//a[contains(text(),'Stop Standard')]",## stop std expath link text
                        "con16st":"//a[contains(text(),'Stop Streaming 16')]",
                        "con32st":"//a[contains(text(),'Stop Streaming 32')]",
                        "con64st":"//a[contains(text(),'Stop Streaming 64')]",
                        "con128st":"//a[contains(text(),'Stop Streaming 128')]",

                        ## Lan name
                         "dhcp_en":"radio_onoff_dhcp",
                         "dhcp_apply":"submit",

                        ## edit usergroup1
                        "edit_ug1":"(//a[contains(text(),'Edit')])[2]",

                        ## network user group page edit default user group

                        ## enable status on user group 1 by name
                        "en_status":"radio_onoff_enabled",
                        ## disable status
                        "dis_status":"(//input[@name='radio_onoff_enabled'])[2]",


                        ## edit the default usergroup
                         "EDIT_UG":"(//a[contains(text(),'Edit')])[11]",
                         ## TCP IP dynamic address selection
                         "sel_dIP":"TCPIP",


                        ## Header compression enable
                        "head_comp":"//tr[9]/td[2]/p/input",##Xpath


                        ## select APN SIM default.by name
                        "sim_def":"(//input[@name='apn_radio'])[2]",##Xpath

                        ## enable auto activation mode by name
                        "en_auto":"radio_onoff_autoactivate",

                        ## disable auto activation mode by xpath
                        "DIS_AUTO":"(//input[@name='radio_onoff_autoactivate'])[2]",


                        ## select standard
                        "sel_std":"//select[@name='primaryconnectionclass']",## select by expath attribute

                        ## select streaming
                        "sel_str8":"//tr[19]/td[2]/select", ##//tr[19]/td[2]/select
                        "sel_str16":"//tr[21]/td[2]/select']", ##//tr[21]/td[2]/select
                        ## Streaming duration on the streaming session log page
                        "str_dur":"//td[2]/p",##Xpath

                        ## apply changes
                        "APPLY_UG":"submit"
    }













