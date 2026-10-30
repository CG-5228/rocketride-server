// =============================================================================
// MIT License
// Copyright (c) 2026 Aparavi Software AG
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

#include "test.h"

TEST_CASE("application::Switch") {
    SECTION("parse takes the on/off words in any case") {
        for (auto word : {"true", "TRUE", "Yes", "on", "ON", "1"})
            REQUIRE(application::Switch::parse(word) == true);
        for (auto word : {"false", "False", "no", "OFF", "off", "0"})
            REQUIRE(application::Switch::parse(word) == false);
    }

    SECTION("parse refuses any other value") {
        for (auto word :
             {"", "true1", "yikes", "nonsense", "t", "n", "2", " on", "on "})
            REQUIRE(application::Switch::parse(word) == NullOpt);
    }

    SECTION("enabled reads an empty or refused value as unset") {
        application::Switch sw{"--test.switch"};
        REQUIRE_FALSE(sw.enabled());
        REQUIRE(sw.enabled(true));

        sw.setValue("on");
        REQUIRE(sw.enabled());
        sw.setValue("Off");
        REQUIRE_FALSE(sw.enabled(true));

        sw.setValue("");
        REQUIRE(sw.enabled(true));
        sw.setValue("true1");
        REQUIRE_FALSE(sw.enabled());
        REQUIRE(sw.enabled(true));
    }

    SECTION("enabled reads a default like a value") {
        application::Switch sw{"--test.switch", "1"};
        REQUIRE(sw.enabled());
    }
}

TEST_CASE("application::Options::check") {
    application::Switch sw{"--test.switch"};
    application::Opt value{"--test.value", "true1"};

    SECTION("passes on/off words, an empty value and other options") {
        for (auto word : {"", "true", "off", "1"}) {
            sw.setValue(word);
            REQUIRE(!application::Options::get().check());
        }
    }

    SECTION("refuses a switch value that is not an on/off word") {
        for (auto word : {"true1", "nonsense", "2", " on"}) {
            sw.setValue(word);
            REQUIRE(application::Options::get().check() == Ec::InvalidParam);
        }
    }
}
