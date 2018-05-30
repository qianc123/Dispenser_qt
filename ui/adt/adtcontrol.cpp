#include "adtcontrol.h"

// ��ʼ�����ƿ� ��һ�ε��òų�ʼ��, �ڶ��ε���ʱ���ص�һ�γ�ʼ���Ľ��
int index = 1;
int ret = 0;
int init_card()
{
	if (index == 1)
	{
		index = 0;

		ret = adt8949_initial();
		
		if (ret < 0)
		{
			QMessageBox::about(NULL, NULL, QStringLiteral("���ƿ���ʼ��ʧ��"));
			return ret;
		}
		else if (ret == 0)
		{
			QMessageBox::about(NULL, NULL, QStringLiteral("���ƿ�δ��װ"));
			return ret;
		}
		else
		{
			qDebug() << ret;
			QMessageBox::about(NULL, NULL, QStringLiteral("���ƿ���ʼ���ɹ�"));

			// ����X��
			adt8949_set_pulse_mode(0, 1, 1, 0, 0);
			adt8949_set_gear(0, 1, 1000);

			// ����Y��
			adt8949_set_pulse_mode(0, 2, 1, 0, 0);
			adt8949_set_gear(0, 2, 1000);

			// ����Z��
			adt8949_set_pulse_mode(0, 3, 1, 0, 0);
			adt8949_set_gear(0, 3, 1000);

			return ret;
		}
	}
	else
	{
		return ret;
	}


}

// ֹͣ��, by axis
void stop_axis(int axis)
{
	adt8949_sudden_stop(0, axis);
}

// ֹͣ������
void stop_allaxis()
{
	adt8949_sudden_stop(0, 3);
	adt8949_sudden_stop(0, 1);
	adt8949_sudden_stop(0, 2);
}

// ��ͣ
void estop()
{
	adt8949_close_card();
}




// ��ȡ�����״̬, by bit
int read_in_bit(int bit)
{
	return adt8949_read_bit(0, bit);
}

// ��ȡ�����״̬, by bit
int read_out_bit(int bit)
{
	return adt8949_get_out(0, bit);
}

// ���������״̬, by bit
void write_out_bit(int bit, int status)
{
	adt8949_write_bit(0, bit, status);
}

// �ı������״̬, by bit
void change_out_bit(int bit)
{
	if (adt8949_get_out(0, bit) == 0)
	{
		adt8949_write_bit(0, bit, 1);
	}
	else
	{
		adt8949_write_bit(0, bit, 0);
	}
}




// ���û�ԭģʽ
void set_home_mode()
{
	QFile file("../config/motor.json");
	if (!file.exists())
	{
		return;
	}

	file.open((QIODevice::ReadOnly));
	QByteArray date = file.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(date);
	QJsonObject obj = doc.object();

	// ��ԭģʽ����
	int m_nHomeDir[3], m_nStop0Active[3], m_nLimitActive[3], m_nStop1Active[3];
	float m_fBackRange[3], m_fEncoderZRange[3], m_fOffset[3];
	
	QJsonObject x_obj = obj.value("HomeMode").toObject().value("X_axis").toObject();
	m_nHomeDir[0]     = x_obj.value("m_nHomeDir").toInt();
	m_nStop0Active[0] = x_obj.value("m_nStop0Active").toInt();
	m_nLimitActive[0] = x_obj.value("m_nLimitActive").toInt();
	m_nStop1Active[0] = x_obj.value("m_nStop1Active").toInt();
	m_fBackRange[0]     = x_obj.value("m_fBackRange").toDouble();
	m_fEncoderZRange[0] = x_obj.value("m_fEncoderZRange").toDouble();
	m_fOffset[0]        = x_obj.value("m_fOffset").toDouble();
	adt8949_SetHomeMode_Ex(0, X_AXIS,
		m_nHomeDir[0],   m_nStop0Active[0],   m_nLimitActive[0], m_nStop1Active[0],
		m_fBackRange[0], m_fEncoderZRange[0], m_fOffset[0]);

	QJsonObject y_obj = obj.value("HomeMode").toObject().value("Y_axis").toObject();
	m_nHomeDir[1]     = y_obj.value("m_nHomeDir").toInt();
	m_nStop0Active[1] = y_obj.value("m_nStop0Active").toInt();
	m_nLimitActive[1] = y_obj.value("m_nLimitActive").toInt();
	m_nStop1Active[1] = y_obj.value("m_nStop1Active").toInt();
	m_fBackRange[1]     = y_obj.value("m_fBackRange").toDouble();
	m_fEncoderZRange[1] = y_obj.value("m_fEncoderZRange").toDouble();
	m_fOffset[1]        = y_obj.value("m_fOffset").toDouble();
	adt8949_SetHomeMode_Ex(0, Y_AXIS,
		m_nHomeDir[1],   m_nStop0Active[1],   m_nLimitActive[1], m_nStop1Active[1],
		m_fBackRange[1], m_fEncoderZRange[1], m_fOffset[1]);

	QJsonObject z_obj = obj.value("HomeMode").toObject().value("Z_axis").toObject();
	m_nHomeDir[2] = z_obj.value("m_nHomeDir").toInt();
	m_nStop0Active[2] = z_obj.value("m_nStop0Active").toInt();
	m_nLimitActive[2] = z_obj.value("m_nLimitActive").toInt();
	m_nStop1Active[2] = z_obj.value("m_nStop1Active").toInt();
	m_fBackRange[2]     = z_obj.value("m_fBackRange").toDouble();
	m_fEncoderZRange[2] = z_obj.value("m_fEncoderZRange").toDouble();
	m_fOffset[2]        = z_obj.value("m_fOffset").toDouble();
	adt8949_SetHomeMode_Ex(0, Z_AXIS,
		m_nHomeDir[2],   m_nStop0Active[2],   m_nLimitActive[2], m_nStop1Active[2],
		m_fBackRange[2], m_fEncoderZRange[2], m_fOffset[2]);

	file.close();
}

// ���û�ԭ�ٶ�
void set_home_speed()
{
	QFile file("../config/motor.json");
	if (!file.exists())
	{
		return;
	}

	file.open((QIODevice::ReadOnly));
	QByteArray date = file.readAll();
	QJsonDocument doc = QJsonDocument::fromJson(date);
	QJsonObject obj = doc.object();

	// ��ԭ�ٶ�����
	float m_fStartSpeed[3], m_fSearchSpeed[3], m_fHomeSpeed[3], m_fAcc[3], m_fZPhaseSpeed[3];
	QJsonObject xs_obj = obj.value("HomeSpeed").toObject().value("X_axis").toObject();
	m_fStartSpeed[0]   = xs_obj.value("m_fStartSpeed").toDouble();
	m_fSearchSpeed[0]  = xs_obj.value("m_fSearchSpeed").toDouble();
	m_fHomeSpeed[0]    = xs_obj.value("m_fHomeSpeed").toDouble();
	m_fAcc[0]          = xs_obj.value("m_fAcc").toDouble();
	m_fZPhaseSpeed[0]  = xs_obj.value("m_fZPhaseSpeed").toDouble();
	adt8949_SetHomeSpeed_Ex(0, X_AXIS, m_fStartSpeed[0], m_fSearchSpeed[0], m_fHomeSpeed[0], m_fAcc[0], m_fZPhaseSpeed[0]);

	QJsonObject ys_obj = obj.value("HomeSpeed").toObject().value("X_axis").toObject();
	m_fStartSpeed[1]   = ys_obj.value("m_fStartSpeed").toDouble();
	m_fSearchSpeed[1]  = ys_obj.value("m_fSearchSpeed").toDouble();
	m_fHomeSpeed[1]    = ys_obj.value("m_fHomeSpeed").toDouble();
	m_fAcc[1]          = ys_obj.value("m_fAcc").toDouble();
	m_fZPhaseSpeed[1]  = ys_obj.value("m_fZPhaseSpeed").toDouble();
	adt8949_SetHomeSpeed_Ex(0, Y_AXIS, m_fStartSpeed[1], m_fSearchSpeed[1], m_fHomeSpeed[1], m_fAcc[1], m_fZPhaseSpeed[1]);

	QJsonObject zs_obj = obj.value("HomeSpeed").toObject().value("X_axis").toObject();
	m_fStartSpeed[2]   = zs_obj.value("m_fStartSpeed").toDouble();
	m_fSearchSpeed[2]  = zs_obj.value("m_fSearchSpeed").toDouble();
	m_fHomeSpeed[2]    = zs_obj.value("m_fHomeSpeed").toDouble();
	m_fAcc[2]          = zs_obj.value("m_fAcc").toDouble();
	m_fZPhaseSpeed[2]  = zs_obj.value("m_fZPhaseSpeed").toDouble();
	adt8949_SetHomeSpeed_Ex(0, Z_AXIS, m_fStartSpeed[2], m_fSearchSpeed[2], m_fHomeSpeed[2], m_fAcc[2], m_fZPhaseSpeed[2]);

	file.close();
}




// �ٶ�, �����, ���Ӽ���
void set_axis_speed(int axis, float speed, float acc, float dec)
{
	adt8949_set_admode(0, axis, 0);
	adt8949_set_startv(0, axis, float(0.1));
	adt8949_set_endv(0, axis, float(0.1));
	adt8949_set_speed(0, axis, speed);
	adt8949_set_acc(0, axis, acc);	// ���ٶ� 
	adt8949_set_dec(0, axis, dec);	// ���ٶ� 
	adt8949_set_jcc(0, axis, 3);	// �Ӽ��ٶ�, ֵԽС, �Ӽ���Ч��Խ����
}

// �ƶ�, ���ٶ�, ���Ӽ���, ��������λ
void move_axis_abs(int axis, float pos, float speed, float acc, float dec)
{
	adt8949_set_admode(0, axis, 0);
	adt8949_set_startv(0, axis, float(0.1));
	adt8949_set_endv(0, axis, float(0.1));
	adt8949_set_speed (0, axis, speed);
	adt8949_set_acc(0, axis, acc);	// ���ٶ� 
	adt8949_set_dec(0, axis, dec);	// ���ٶ� 
	adt8949_set_jcc(0, axis, 3);	// �Ӽ��ٶ�, ֵԽС, �Ӽ���Ч��Խ����
	adt8949_abs_pmove(0, axis, pos);
}

// �ƶ�, ���ٶ�, ���Ӽ���, ��������λ
void move_axis_offset(int axis, float distance, float speed, float acc, float dec)
{
	adt8949_set_admode(0, axis, 0);
	adt8949_set_startv(0, axis, float(0.1));
	adt8949_set_endv(0, axis, float(0.1));
	adt8949_set_speed(0, axis, speed);
	adt8949_set_acc(0, axis, acc);	// ���ٶ� 
	adt8949_set_dec(0, axis, dec);	// ���ٶ� 
	adt8949_set_jcc(0, axis, 3);	// �Ӽ��ٶ�, ֵԽС, �Ӽ���Ч��Խ����
	adt8949_abs_pmove(0, axis, distance);
}

// �����˶�, ������(0+, 1-), ���ٶ�, ���Ӽ���, ��������λ
void move_axis_continue(int axis, int dir, float speed, float acc, float dec)
{
	adt8949_set_admode(0, axis, 0);
	adt8949_set_startv(0, axis, float(0.1));
	adt8949_set_endv(0, axis, float(0.1));
	adt8949_set_speed(0, axis, speed);
	adt8949_set_acc(0, axis, acc);	// ���ٶ� 
	adt8949_set_dec(0, axis, dec);	// ���ٶ� 
	adt8949_set_jcc(0, axis, 3);	// �Ӽ��ٶ�, ֵԽС, �Ӽ���Ч��Խ����
	adt8949_continue_move(0, axis, dir);
}




// �ж��Ƿ��˶� by axis
bool axis_isMoving(int axis)
{
	int state = 0;
	adt8949_get_status(0, axis, &state);
	return state;
}

// �ȴ��˶�����
void wait_finish(int axis)
{
	int state = 0;
	while (true)
	{
		Sleep(1);
		adt8949_get_status(0, axis, &state);
		
		if (state==0)
		{
			break;
		}
	} 	
}
