// BreakControl.hh --- window for the micropause
//
// Copyright (C) 2001, 2002 Rob Caelers <robc@krandor.org>
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Id$
//

#ifndef BREAKCONTROL_HH
#define BREAKCONTROL_HH

#include <stdio.h>
#include <sigc++/object.h>

#include "BreakInterface.hh"
#include "BreakResponseInterface.hh"
#include "GUIControl.hh"

class PreludeWindow;
class BreakWindowInterface;
class PreludeWindowInterface;
class TimerInterface;
class ControlInterface;
class GUIFactoryInterface;

class BreakControl :
  public BreakInterface,
  public BreakResponseInterface
{
public:
  BreakControl(GUIControl::BreakId id, ControlInterface *control,
               GUIFactoryInterface *factory, TimerInterface *timer);
  virtual ~BreakControl();

  // BreakInterface
  void start_break();
  void force_start_break();
  void stop_break();
  void set_prelude_text(string text);
  bool need_heartbeat();
  void heartbeat();
  BreakState get_break_state();

  // Configuration
  void set_force_after_preludes(bool f);
  void set_max_preludes(int m);
  void set_insist_break(bool i);
  void set_ignorable_break(bool i);
  
  // BreakResponseInterface
  void postpone_break();
  void skip_break();

  //
  void break_window_start();
  void break_window_stop();
  void prelude_window_start();
  void prelude_window_stop();

  void collect_garbage();
  
private:
  enum BreakStage { STAGE_NONE,
                    STAGE_SNOOZED,
                    STAGE_PRELUDE,
                    STAGE_TAKING
  };

  void update_prelude_window();
  void update_break_window();
  void goto_stage(BreakStage stage);
  void suspend_break();

private:
  //! ID of the break.
  GUIControl::BreakId break_id;
  
  //! The Controller.
  ControlInterface *core_control;

  //! GUI Factory used to create the break/prelude windows.
  GUIFactoryInterface *gui_factory;
  
  //! Interface to the timer controlling the break.
  TimerInterface *break_timer;
  
  //! Interface to the break window.
  BreakWindowInterface *break_window;

  //! Interface to the prelude window.
  PreludeWindowInterface *prelude_window;

  //! "Prompt" text in the prelude window.
  string prelude_text;
  
  //! Current stage in the break.
  BreakStage break_stage;

  //! This is a final prompt, forcing break after this prelude
  bool final_prelude;
  
  //! How long is the prelude active.
  int prelude_time;

  //! (User initiated/seld-inflicted) forced break (i.e. RestBreak now)
  bool forced_break;

  //! How many times have we preluded (since the limit was reached)
  int prelude_count;

  //! After how many preludes do we force a break or give up?
  int number_of_preludes;

  //! After 'number_of_preludes' do we force a break ?
  bool force_after_prelude;
  
  //! Can the user continue during break?
  bool insist_break;

  //! Can the use explicitly ignore the break?
  bool ignorable_break;

  //! Destroy break window on next heartbeat?
  bool break_window_destroy;

  //! Destroy prelude window on next heartbeat?
  bool prelude_window_destroy;
};

#endif // BREAKCONTROL_HH
