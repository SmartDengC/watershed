#pragma once

#include <QtWidgets/QWidget>
#include "ui_qt_1.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

class Qt_1 : public QWidget
{
	Q_OBJECT

public:
	Qt_1(QWidget *parent = Q_NULLPTR);

private:
	Ui::Qt_1Class ui;

private:
	QPushButton* first_button;
	QPushButton* second_button;
	QLabel* origin_label;
	QLabel* shift_label;
	QLabel* binary_label;
	QLabel* gray_label;
	QLabel* image;
	QLabel* origin2_label;
	QLabel* distance_label;
	QLabel* binary2_label;
	QLabel* watershed_label;
	QLabel* watershed_number_label;
	QLineEdit* count2_label;
	QTextEdit * e1_edit;

	// ¶ÀÁ¢²Ù×÷°´Å¥
	QPushButton* origin_button;
	QPushButton* origin2_button;
	QPushButton* shift_button;
	QPushButton* gray_button;
	QPushButton* binary_button;
	QPushButton* distance_button;
	QPushButton* binary2_button;
	QPushButton* watershed_button;
	QPushButton* watershed_number_button;



private slots:
	//
};
