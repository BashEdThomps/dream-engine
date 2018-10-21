/*
 * Event
 *
 * Created: 18/12/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include <string>
#include <map>

#include "../Common/DreamObject.h"
#include <json.hpp>

using std::string;
using std::map;
using nlohmann::json;

namespace Dream
{
    class SceneObjectRuntime;

    class Event
            : public DreamObject
    {

    private:
      json mData;
      SceneObjectRuntime* mSender;

    public:
      Event(SceneObjectRuntime*,string);
      ~Event();

      SceneObjectRuntime* getSender() const;
      string getType() const;

      void setString(string,string);
      string getString(string) const;

      void setJson(string, json);
      json getJson(string) const;

      string getData() const;

      void setNumber(string name, double number);
      double getNumber(string name);

    }; // End of Event

} // End of Dream
