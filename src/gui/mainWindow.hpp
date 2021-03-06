/*
 *  Copyright (C) 2010-2021 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef MAINWINDOW_HPP_
#define MAINWINDOW_HPP_

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QShortcut>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtCore/QTranslator>
#include <QtCore/QPoint>
#include "settings.h"
#include "jstick.h"
#include "mainWindow.hh"
#include "wdgScreen.hpp"
#include "wdgStatusBar.hpp"
#include "wdgToolBar.hpp"

class qtHelper {
	public:
		static void widget_set_visible(void *wdg, bool mode);
		static void pushbutton_set_checked(void *btn, bool mode);
		static void checkbox_set_checked(void *cbox, bool mode);
		static void slider_set_value(void *slider, int value);
		static void spinbox_set_value(void *sbox, int value);
};
class timerEgds : public QTimer {
		Q_OBJECT

	private:
		enum with_emu_pause {
			EGDS_PAUSE,
			EGDS_TURN_OFF,
			EGDS_TOTALS
		};
		struct _calls {
			int count;
		} calls[EGDS_TOTALS];

	public:
		timerEgds(QObject *parent = 0);
		~timerEgds();

	public:
		void set_fps(void);
		void stop_unnecessary(void);
		void start_pause(void);
		void stop_pause(void);
		void start_rwnd(void);
		void stop_rwnd(void);
		void start_ff(void);
		void stop_ff(void);
		void start_turn_off(void);
		void stop_turn_off(void);

	private:
		void _start(void);
		void _start_with_emu_thread_pause(enum with_emu_pause type);
		void _stop(BYTE is_necessary);
		void _stop_with_emu_thread_continue(enum with_emu_pause type, BYTE is_necessary);
		void _etc(enum with_emu_pause type);

	private slots:
		void s_draw_screen(void);
};
class mainWindow : public QMainWindow, public Ui::mainWindow {
		Q_OBJECT

	public:
		struct _qaction_shcut_extern {
			QAction *mode_auto;
			QAction *mode_ntsc;
			QAction *mode_pal;
			QAction *mode_dendy;
			QAction *scale_1x;
			QAction *scale_2x;
			QAction *scale_3x;
			QAction *scale_4x;
			QAction *scale_5x;
			QAction *scale_6x;
			QAction *interpolation;
			QAction *integer_in_fullscreen;
			QAction *stretch_in_fullscreen;
			QAction *audio_enable;
			QAction *save_settings;
			struct _qaction_shcut_extern_rwnd {
				QAction *active;
				QAction *step_backward;
				QAction *fast_backward;
				QAction *play;
				QAction *pause;
				QAction *fast_forward;
				QAction *step_forward;
			} rwnd;
		} qaction_shcut;
		// ext_gfx_draw_screen
		timerEgds *egds;
		wdgScreen *screen;
		wdgStatusBar *statusbar;
		wdgToolBar *toolbar;

	private:
		struct _shcjoy {
			bool enabled;
			QTimer *timer;
			_js_sch sch;
			DBWORD shortcut[SET_MAX_NUM_SC];
		} shcjoy;
		QShortcut *shortcut[SET_MAX_NUM_SC];
		QTranslator *translator;
		QTranslator *qtTranslator;
		bool toggle_gui_in_window;
		QRect geom, mgeom;

	public:
		mainWindow();
		~mainWindow();

	signals:
		void et_gg_reset(void);
		void et_vs_reset(void);
		void et_external_control_windows_show(void);
		void fullscreen(bool state);

	protected:
#if defined (_WIN32)
		bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif
		bool eventFilter(QObject *obj, QEvent *event);
		void changeEvent(QEvent *event);
		void closeEvent(QCloseEvent *event);
		void moveEvent(QMoveEvent *event);
		void resizeEvent(QResizeEvent *event);

	private:
		void retranslateUi(mainWindow *mainWindow);

	public:
		void update_window(void);
		void update_recording_widgets(void);
		void set_language(int lang);
		void shcjoy_start(void);
		void shcjoy_stop(void);
		void control_visible_cursor(void);
		void make_reset(int type);
		void change_rom(const uTCHAR *rom);
		void state_save_slot_set(int slot, bool on_video);
		void shortcuts(void);
		bool is_rwnd_shortcut_or_not_shcut(const QKeyEvent *event);
		void update_gfx_monitor_dimension(void);
		void set_save_slot_tooltip(BYTE slot, char *buffer);

	private:
		void connect_menu_signals(void);
		void connect_action(QAction *action, const char *member);
		void connect_action(QAction *action, int value, const char *member);
		void connect_shortcut(QAction *action, int index);
		void connect_shortcut(QAction *action, int index, const char *member);

	private:
		void update_menu_file(void);
		void update_menu_nes(void);
		void update_menu_tools(void);
		void update_menu_state(void);

	private:
		void ctrl_disk_side(QAction *action);
		int is_shortcut(const QKeyEvent *event);

	public slots:
		void s_set_fullscreen(void);
		void s_set_vs_window(void);

	private slots:
		void s_open(void);
		void s_apply_patch(void);
		void s_open_recent_roms(void);
		void s_open_working_folder(void);
		void s_quit(void);
		void s_turn_on_off(void);
		void s_make_reset(void);
		void s_insert_coin(void);
		void s_disk_side(void);
		void s_eject_disk(void);
		void s_start_stop_audio_recording(void);
#if defined (WITH_FFMPEG)
		void s_start_stop_video_recording(void);
#endif
		void s_save_screenshot(void);
		void s_save_screenshot_1x(void);
		void s_pause(void);
		void s_fast_forward(void);
		void s_toggle_gui_in_window(void);
		void s_open_settings(void);
		void s_state_save_slot_action(void);
		void s_state_save_slot_incdec(void);
		void s_state_save_slot_set(void);
		void s_state_save_file(void);
		void s_state_load_file(void);
		void s_help(void);

	private slots:
		void s_fullscreen(bool state);
		void s_shcjoy_read_timer(void);

	private slots:
		void s_shcut_mode(void);
		void s_shcut_scale(void);
		void s_shcut_interpolation(void);
		void s_shcut_integer_in_fullscreen(void);
		void s_shcut_stretch_in_fullscreen(void);
		void s_shcut_audio_enable(void);
		void s_shcut_save_settings(void);
		void s_shcut_rwnd_active_deactive_mode(void);
		void s_shcut_rwnd_step_backward(void);
		void s_shcut_rwnd_fast_backward(void);
		void s_shcut_rwnd_play(void);
		void s_shcut_rwnd_pause(void);
		void s_shcut_rwnd_fast_forward(void);
		void s_shcut_rwnd_step_forward(void);

	private slots:
		void s_et_gg_reset(void);
		void s_et_vs_reset(void);
		void s_et_external_control_windows_show(void);
};

#endif /* MAINWINDOW_HPP_ */
