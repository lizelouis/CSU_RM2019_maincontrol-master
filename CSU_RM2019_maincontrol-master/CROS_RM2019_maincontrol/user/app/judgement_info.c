/****************************************************************************
 *  Copyright (C) 2018 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/
/** @file judgement_info.c
 *  @version 1.0
 *  @date June 2017
 *
 *  @brief the information from judgement system
 *
 *  @copyright 2017 DJI RoboMaster. All rights reserved.
 *
 */

#include "judgement_info.h"
#include "communicate.h"
//#include "comm_task.h"
#include "protocol.h"
//#include "bsp_uart.h"
#include "data_fifo.h"
#include "string.h"


/* data send (forward) */
/* data receive */
receive_judge_t judge_recv_mesg;


extern int8_t recv_pc_glb;
extern int8_t glb_err_exit;

/**
  * @brief    get judgement system message
  */


//for debug
uint16_t cmd_ID = 0;
uint16_t data_Length = 0;

void judgement_data_handler(uint8_t *p_frame)//裁判系统数据解析
{
  //从fifo中提取相应数据
  frame_header_t *p_header = (frame_header_t*)p_frame;

  uint16_t data_length = p_header->data_length;
  uint16_t cmd_id      = *(uint16_t *)(p_frame + HEADER_LEN);
  uint8_t *data_addr   = p_frame + HEADER_LEN + CMD_LEN;//数据帧头地址
  
  memcpy(p_header, p_frame, HEADER_LEN);	
  //uint8_t forward_flag = 1;
  
	//for_debug
	cmd_ID = cmd_id;
	data_Length = data_length;
	
  //根据命令码将数据装入对应的用户数据结构
  switch (cmd_id)
  {
    case GAME_INFO_ID:
    {
      memcpy(&judge_recv_mesg.game_information, data_addr, data_length);
      if (judge_recv_mesg.game_information.game_process == 3)  //5 seconds count down
      {
        //shoot.shoot_bullets = 0;
      }
      
      if (judge_recv_mesg.game_information.game_process == 1)
      {
        if (judge_recv_mesg.game_information.stage_remain_time < 240)
        {
          //if (recv_pc_glb == 0)
          //{
            //glb_err_exit = 1;
          //}
        }
      }
      else
      {
        //recv_pc_glb  = 0;
        //glb_err_exit = 0;
      }
    }
    break;

    case REAL_BLOOD_DATA_ID:
      memcpy(&judge_recv_mesg.blood_changed_data, data_addr, data_length);
    break;

    case REAL_SHOOT_DATA_ID:
      memcpy(&judge_recv_mesg.real_shoot_data, data_addr, data_length);
    break;
    
//    case REAL_POWER_DATA_ID:
//      memcpy(&judge_recv_mesg.power_heat_data, data_addr, data_length);
//    break;
    
    case REAL_FIELD_DATA_ID:
      memcpy(&judge_recv_mesg.rfid_data, data_addr, data_length);
    break;

    case GAME_RESULT_ID:
      memcpy(&judge_recv_mesg.game_result_data, data_addr, data_length);
    break;

    case GAIN_BUFF_ID:
      memcpy(&judge_recv_mesg.get_buff_data, data_addr, data_length);
    break;
    
    case ROBOT_POS_DATA_ID:
      memcpy(&judge_recv_mesg.robot_pos_data, data_addr, data_length);
    break;
    
    default:
      //forward_flag = 0;
    break;
  }
  
  /* forward data */
//  if (forward_flag)//上一步成功，进行下一步
//  {
//    data_packet_pack(cmd_id, data_addr, data_length, UP_REG_ID);
//    osSignalSet(pc_unpack_task_t, PC_UART_TX_SIGNAL);
//  }
		
}

