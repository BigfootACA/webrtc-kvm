/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#ifndef WEBRTC_CLIENT_LOGIN_H
#define WEBRTC_CLIENT_LOGIN_H
#include<QLabel>
#include<QDialog>
#include<QLineEdit>
#include<QCheckBox>
#include<QGridLayout>
#include<QDialogButtonBox>

class LoginDialog:public QDialog{
	Q_OBJECT
	public:
		explicit LoginDialog(QWidget*parent=nullptr);
		void Retranslate();
		~LoginDialog()override;
		QLabel*label_title;
		QLineEdit*edit_username;
		QLineEdit*edit_password;
		QCheckBox*check_save;
		QCheckBox*check_auto;
	protected slots:
		void onSaveCheckStateChanged(Qt::CheckState state);
		void onAutoCheckStateChanged(Qt::CheckState state);
		void onTextChanged(const QString&text);
	private:
		QGridLayout*grid;
		QLabel*label_username;
		QLabel*label_password;
		QHBoxLayout*check_box;
		QDialogButtonBox*button_box;
		QPushButton*button_ok;
		QPushButton*button_cancel;
};
#endif
