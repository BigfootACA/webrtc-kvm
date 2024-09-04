/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include"login.h"
#include<QSpacerItem>
#include<QHBoxLayout>
#include<QPushButton>

using QSizePolicy::Policy::Fixed;
using QSizePolicy::Policy::Minimum;
using QSizePolicy::Policy::Expanding;

LoginDialog::LoginDialog(QWidget*parent):QDialog(parent){
	resize(340,170);
	setSizePolicy(QSizePolicy(Fixed,Fixed));
	setMinimumSize(QSize(340,170));
	setMaximumSize(QSize(340,170));
	grid=new QGridLayout(this);

	label_title=new QLabel(this);
	label_title->setAlignment(Qt::AlignCenter);
	grid->addWidget(label_title,0,2,1,2);

	grid->addItem(new QSpacerItem(20,40,Minimum,Expanding),1,2,1,2);

	label_username=new QLabel(this);
	grid->addWidget(label_username,2,2,1,1);
	edit_username=new QLineEdit(this);
	connect(edit_username,&QLineEdit::textChanged,this,&LoginDialog::onTextChanged);
	grid->addWidget(edit_username,2,3,1,1);

	grid->addItem(new QSpacerItem(20,40,Minimum,Expanding),3,2,1,2);

	label_password=new QLabel(this);
	grid->addWidget(label_password,4,2,1,1);
	edit_password=new QLineEdit(this);
	connect(edit_password,&QLineEdit::textChanged,this,&LoginDialog::onTextChanged);
	edit_password->setEchoMode(QLineEdit::Password);
	grid->addWidget(edit_password,4,3,1,1);

	grid->addItem(new QSpacerItem(20,40,Minimum,Expanding),6,2,1,2);

	check_box=new QHBoxLayout();
	check_box->addItem(new QSpacerItem(40,20,Expanding,Minimum));

	check_save=new QCheckBox(this);
	connect(check_save,&QCheckBox::checkStateChanged,this,&LoginDialog::onSaveCheckStateChanged);
	check_box->addWidget(check_save);
	check_box->addItem(new QSpacerItem(40,20,Expanding,Minimum));

	check_auto=new QCheckBox(this);
	connect(check_auto,&QCheckBox::checkStateChanged,this,&LoginDialog::onAutoCheckStateChanged);
	check_box->addWidget(check_auto);
	check_box->addItem(new QSpacerItem(40,20,Expanding,Minimum));

	grid->addLayout(check_box,9,2,2,2);

	button_box=new QDialogButtonBox(this);
	button_box->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	button_box->setCenterButtons(true);
	button_ok=button_box->button(QDialogButtonBox::Ok);
	button_cancel=button_box->button(QDialogButtonBox::Cancel);
	button_ok->setEnabled(false);
	connect(button_ok,&QPushButton::clicked,this,&QDialog::accept);
	connect(button_cancel,&QPushButton::clicked,this,&QDialog::reject);
	grid->addWidget(button_box,11,2,1,2);

	Retranslate();
}

void LoginDialog::Retranslate(){
	setWindowTitle("Login");
	label_title->setText("Login into WebRTC-KVM");
	label_username->setText("Username");
	label_password->setText("Password");
	edit_username->setPlaceholderText("Username");
	edit_password->setPlaceholderText("Password");
	check_save->setText("Save Password");
	check_auto->setText("Auto Login");
}

LoginDialog::~LoginDialog(){

}

void LoginDialog::onSaveCheckStateChanged(Qt::CheckState state){
	if(!check_save->isChecked()&&check_auto->isChecked())
		check_auto->setChecked(false);
}

void LoginDialog::onAutoCheckStateChanged(Qt::CheckState state){
	if(check_auto->isChecked()&&!check_save->isChecked())
		check_save->setChecked(true);
}

void LoginDialog::onTextChanged(const QString&text){
	bool en=true;
	if(edit_username->isEnabled()&&edit_username->text().isEmpty())en=false;
	if(edit_password->isEnabled()&&edit_password->text().isEmpty())en=false;
	button_ok->setEnabled(en);
}
