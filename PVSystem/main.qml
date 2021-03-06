/****************************************************************************
 *
 *   Copyright (C) 2017 Marco Rossi. All rights reserved.
 *   Author: Marco Rossi <info@nerinformatica.it>
 *   Contributor: Luciano Neri <l.neri@nerinformatica.it>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted under the LGPL v3 license
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import "basics.js" as Basics

ApplicationWindow {

    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("PVSystem")
    color: "#e8e8e8"

    onClosing:{
        //terminating the varmanager and emu thread before closing the main window
        emonvars.terminate()
        pvemu.terminate()
    }

    //top black status bar
    StatusBar{
        id: statBar
        xpos: 0
        ypos: 0
        w: window.width
        h: window.height / 16
    }




    //rectangle container containing all the items
    Rectangle{
        id: controlsContainer
        x: 0
        y: statBar.h
        width: window.width
        height: window.height - statBar.h
        color: "#f4f4f4"

        //left quick status view
        Rectangle{
            id: leftStatus
            y: 45
            anchors.margins: 30
            anchors.left: parent.left
            color: "white"
            anchors.leftMargin: 20
            width: centerStatus.width/2
            height: 108
            Image{
                height: parent.height/1.3
                width: height
                anchors.centerIn: parent
                source:{

                    if(networkvars.getIP() === ""){
                        "images/error.png"
                    }
                    else{
                        "images/ok.png"
                    }

                }

            }
        }

        //general information tab
        Rectangle{
            id: mainInfoTab
            y: 171
            anchors.margins: 30
            anchors.left: parent.left
            color: "white"
            anchors.leftMargin: 20
            width: centerStatus.width/2
            height: 234
            Label{
                id: powerLabel
                y: 20
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 20
                font.bold: true
                text: "Power: " + emonvars.getValue(2) + "W"
                color: "#636265"
            }
            Label{
                id: energyLabel
                y: powerLabel.y+powerLabel.height + 20
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 20
                font.bold: true
                text: "Energy: " + emonvars.getValue(1) + "A"
                color: "#636265"
            }
            Image{
                y: energyLabel.y + energyLabel.height + 20
                anchors.horizontalCenter: parent.horizontalCenter
                source:{

                    if(networkvars.get(4) === "Veneto"){"images/regions/veneto.png"}
                    if(networkvars.get(4) === "Toscana"){"images/regions/toscana.png"}
                    if(networkvars.get(4) === "Lazio"){"images/regions/lazio.png"}
                    if(networkvars.get(4) === "Umbria"){"images/regions/umbria.png"}
                    if(networkvars.get(4) === "Sardegna"){"images/regions/sardegna.png"}
                    if(networkvars.get(4) === "Campania"){"images/regions/campania.png"}
                    if(networkvars.get(4) === "Abbruzzo"){"images/regions/abbruzzo.png"}
                    if(networkvars.get(4) === "Calabria"){"images/regions/calabria.png"}
                    if(networkvars.get(4) === "Molise"){"images/regions/molise.png"}
                    if(networkvars.get(4) === "Basilicata"){"images/regions/basilicata.png"}
                    if(networkvars.get(4) === "Lombardia"){"images/regions/lombardia.png"}
                    if(networkvars.get(4) === "Sicilia"){"images/regions/sicilia.png"}
                    if(networkvars.get(4) === "Emilia Romagna"){"images/regions/emilia.png"}
                    if(networkvars.get(4) === "Marche"){"images/regions/marche.png"}
                    if(networkvars.get(4) === "Val D'Aosta"){"images/regions/aosta.png"}
                    if(networkvars.get(4) === "Friuli Venezia Giulia"){"images/regions/friuli.png"}
                    if(networkvars.get(4) === "Liguria"){"images/regions/liguria.png"}
                    if(networkvars.get(4) === "Piemonte"){"images/regions/piemonte.png"}
                    if(networkvars.get(4) === "Trentino"){"images/regions/trentino.png"}

                }
            }
        }
        //indicator controls tab
        Rectangle{
            id: indicatorControls
            y: 45
            anchors.margins: 30
            anchors.right: parent.right
            color: "#fcfcfc"
            anchors.rightMargin: 20
            width: centerStatus.width/2
            height: 360
            IndicatorControl{
                id: iControl
                w: parent.width/1.5
            }
        }

        //main indicator
        Rectangle{
            id: centerStatus
            anchors.centerIn: parent
            color: "#fcfcfc"
            width: parent.width/2 * 0.9
            height: width
            Image{
                anchors.centerIn: parent
                source: "images/indicator_slide.png"

                Indicator{
                    id: spinningIndicator
                    posx: centerStatus.width/2
                    posy: centerStatus.height/4 + 50
                }

                Timer{
                    id: updatebar
                    repeat: true
                    interval: 2000
                    running: true
                    //will get the past position and new position
                    onTriggered:{

                        if(iControl.mode == 0){
                            spinningIndicator.posangle = 360 * emonvars.getPosition()
                            spinningIndicator.watts = emonvars.getValue(2)
                        }
                        else{
                            spinningIndicator.posangle = 360 * emonvars.getConsumePosition(emonvars.getValue(1))
                        }

                        //updating info values and graph
                        infoPage.updateValues()
                        infoPage.updateGraph()

                    }
                }

            }
        }

        /*
        ComboBox{
            id: indicatorMode
            anchors.horizontalCenter: parent.horizontalCenter
            y: 40
            width: window.width/3
            model:["Current Inverter Power", "Consume Status"]

        }
        */

        Drawer{
            id: drawer
            width: parent.width
            height: parent.height - botBar.h
            edge: Qt.TopEdge
            background: Rectangle{
                color: "#f4f4f4"
            }
            Settings{
                id: settingsPage
                w: window.width
                h: window.height
            }
        }

       Drawer{
           id: infoDrawer
           width: parent.width
           height: parent.height - botBar.h
           edge: Qt.TopEdge
           background: Rectangle{
               color: "#f4f4f4"
           }
           Info{
               id: infoPage
               w: window.width
               h: window.height
           }
       }

       Drawer{
           id: netDrawer
           width: parent.width
           height: parent.height - botBar.h
           edge: Qt.TopEdge
           background: Rectangle{
               color: "#f4f4f4"
           }
           NetSettings{
               id: netPage
               w: window.width
               h: window.height
           }
       }

       Drawer{
           property int pHeight: parent.height/8

           interactive: false
           id: keyboard
           width: parent.width
           height: parent.height/2 + parent.height/8
           edge: Qt.BottomEdge
           background: Rectangle{
               color: "#f4f4f4"
           }
           Label{
               id: tempInput
               font.pixelSize: 20
               color: "#636265"
               text: ""
               y: height
               anchors.horizontalCenter: parent.horizontalCenter

           }

           Keyboard{
               span: parent.pHeight
               id: vkeyboard
               w: window.width
               h: window.height
           }
       }

       Drawer{
           id: bottomBarMenu
           width: parent.width
           height: controlsContainer.height/5
           edge: Qt.BottomEdge
           //bottom bar with button controls
           BottomBar{
               id: botBar
               w: window.width
               h: controlsContainer.height/5
               anchors.bottom: parent.bottom
           }
       }

    }
}
