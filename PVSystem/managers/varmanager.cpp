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

/**
    PVSystem, varmanager.cpp
    Purpose: Manages and updates QML Variables asynchronously

    @author Marco Rossi
    @version 1.0 09/05/17
*/

#include "varmanager.h"
#include "../protocols/remoteserver.h"
#include <utilities/jsonparser.h>
#include <thread>
#include <QTimer>
#include <QObject>
#include <QQuickView>
#include <QQuickItem>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

//hash tables id->name & name->id
QHash<QString, QString> hash_a;
QHash<QString, QString> hash_b;

RemoteServer r;
RemoteServer up;

int loaded = 0;

//structure containing all the important values
struct vars{
    QString apiKey = "1b15eb3ce081a80829e78acb83c5004a";
    QString inverterPower;
    QString panelsAmount;
    QString wattPeak;
    QString tiltAngle;
    QString irradiation;
    QString panelYield;
    QString azimuthAngle;
    QString percentage;
    QString currentEnergy;
    QString maxEnergy = "1000";
}vars;

varmanager::varmanager()
{
    //building hashes
    buildHash(vars.apiKey);
    //initilization of struct vars
    vars.inverterPower = r.getValue(vars.apiKey, hash_b["power"]);
    vars.panelsAmount = r.getValue(vars.apiKey, hash_b["panels"]);
    vars.wattPeak = r.getValue(vars.apiKey, hash_b["peak"]);
    vars.tiltAngle = r.getValue(vars.apiKey, hash_b["t_angle"]);
    vars.irradiation = r.getValue(vars.apiKey, hash_b["irradiation"]);
    vars.panelYield = r.getValue(vars.apiKey, hash_b["yield"]);
    vars.azimuthAngle = r.getValue(vars.apiKey, hash_b["azimuth"]);
    vars.percentage = r.getValue(vars.apiKey, hash_b["percentage"]);
    vars.currentEnergy = r.getValue(vars.apiKey, hash_b["energy"]);
    //notify the other methods that the vars have been loaded
    loaded = 1;
}

void varmanager::run()
{
    //reading/writing every 2 seconds

    while(true)
    {
        if(vars.apiKey.isEmpty() == false)
        {

                vars.inverterPower = r.getValue(vars.apiKey, hash_b["power"]);
                vars.panelsAmount = r.getValue(vars.apiKey, hash_b["panels"]);
                vars.wattPeak = r.getValue(vars.apiKey, hash_b["peak"]);
                vars.tiltAngle = r.getValue(vars.apiKey, hash_b["t_angle"]);
                vars.irradiation = r.getValue(vars.apiKey, hash_b["irradiation"]);
                vars.panelYield = r.getValue(vars.apiKey, hash_b["yield"]);
                vars.azimuthAngle = r.getValue(vars.apiKey, hash_b["azimuth"]);
                vars.percentage = r.getValue(vars.apiKey, hash_b["percentage"]);
                vars.currentEnergy = r.getValue(vars.apiKey, hash_b["energy"]);
                qDebug() << vars.wattPeak << endl;


        }
        else
        {
            qDebug() << "API Key not found" << endl;
        }

        sleep(1);
    }

}

/**
    Notifies the user changes by setting the mutex to 1

    @param id ID of the changed parameter.
    @param newval New value
*/
void varmanager::notifyChange(int id, QString newval)
{

        QString url;
        QString ret;

        switch(id)
        {
            case 0:
                vars.apiKey = newval;
                break;
            case 1:
                url = "https://emoncms.org/input/post?json={panels:" + newval+ "}&apikey=" + vars.apiKey;
                ret = up.getResponse(url);
                break;
            case 2:
                url = "https://emoncms.org/input/post?json={t_angle:" + newval+ "}&apikey=" + vars.apiKey;
                ret = up.getResponse(url);
                break;
            case 3:
                url = "https://emoncms.org/input/post?json={peak:" + newval+ "}&apikey=" + vars.apiKey;
                ret = up.getResponse(url);
                break;
            case 4:
                url = "https://emoncms.org/input/post?json={azimuth:" + newval+ "}&apikey=" + vars.apiKey;
                ret = up.getResponse(url);
                break;
            default:
                break;
        }


}

/**
    Builds the hash tables

    @param api Read/Write API Key.
*/
void varmanager::buildHash(QString api)
{
    hash_a = r.generateHash("http://emoncms.org/feed/list.json?apikey=" + api, "id", "name");
    hash_b = r.generateHash("http://emoncms.org/feed/list.json?apikey=" + api, "name", "id");
}

/**
    Gets the required value

    @return POSITION
*/

float varmanager::getPosition()
{
    if(loaded = 1)
    {
        float retval = vars.inverterPower.toFloat() / vars.wattPeak.toFloat();
        return retval;
    }
    else
    {
        return 0.5;
    }

}

/**
    Gets the relative position of the indicator based on the consume

    @param consume Consumed energy
    @return indicator position 0.0 - 1
*/

float varmanager::getConsumePosition(int consume)
{
    if(loaded = 1)
    {
        int remenergy = vars.inverterPower.toInt() - consume;
        if(remenergy > 0){
            float retval = 0.5 + remenergy / vars.wattPeak.toFloat();
            return retval;
        }
        else if(remenergy < 0){

            float retval = 0.5 - (remenergy*-1) / vars.wattPeak.toFloat();
            return retval;
        }
        else{
            return 0.5;
        }
    }
    else
    {
        return 0.5;
    }

}

/**
    Gets the required value
    @param id Item id
    @return required value
*/
QString varmanager::getValue(int id)
{
    switch(id)
    {
    case 0:
        return vars.azimuthAngle;
        break;
    case 1:
        return vars.currentEnergy;
        break;
    case 2:
        return vars.inverterPower;
        break;
    case 3:
        return vars.irradiation;
        break;
    case 4:
        return vars.panelsAmount;
        break;
    case 5:
        return vars.panelYield;
        break;
    case 6:
        return vars.percentage;
        break;
    case 7:
        return vars.tiltAngle;
        break;
    case 8:
        return vars.wattPeak;
        break;
    case 9:
        return vars.apiKey;
        break;
    case 10:
        return vars.maxEnergy;
        break;
    default:
        return "";
        break;

    }

}

/**
    Sets and updates the local irradiance

    @param region Local region
*/
void varmanager::setIrradiance(QString region)
{
    JsonParser jp;
    //parsing annual irradiation
    float raw = jp.ParseLocal("C:/Users/Marco/Desktop/ASLProject/PVSystem/italy.json", region.toLower()).toFloat();
    //calculating daily irradiation
    float daily = raw/365;
    //converting value to QString
    QString n_irr = QString::number(daily);
    //sending request to EmonCMS
    QString url = "https://emoncms.org/input/post?json={irradiation:" + n_irr + "}&apikey=" + vars.apiKey;
    QString ret = up.getResponse(url);
}






