/**
 * @file ytime.h
 * @author 殉道者  wcjbyjx@gmail.com
 * @version 0.0.1
 * @date 2022-02-22 11:50
 * @copyright Copyright © 2014-2022 weichijunbo.
 * @brief 计时器
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-02-22 <td>0.0.1     <td>殉道者     <td>内容
 * </table>
 * */
#pragma once
#include <iostream>
#include <thread>
#include <chrono>
/**
* @class <Timer> [ytime.h] [<ytime>]
* @brief 基于STL的chrono实现计时器，计算函数，代码段执行时间
* @note
* 基于STL的chrono实现计时器，计算函数，代码段执行时间。创建Timer对象时为起始时间，Timer对象析构时
* 获取当前时间并计算时间差，即可获得执行时间。结合大括号{}和类的生命周期来使用，可以很方便的计算函数、代码段的执行时间。
* 目前只实现了毫秒级别精度的计算器，足够开发使用。
**/
class Timer
{
private:
    std::chrono::system_clock::time_point start; /**<起始时间 */
    std::chrono::system_clock::time_point end;   /**<截至时间 */
   
    std::chrono::duration<double> duration; 
public:
    /**
    * @brief  记录构造函数被调用时的时间
    */
    Timer()
    {
        start = std::chrono::high_resolution_clock::now();
    }
    /**
     * @brief 调用析构函数时，获取当前时间，并计算对象从被构造到析构所经过时间（毫秒）
     * 
     */
    ~Timer() 
    {
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        
        double ms = duration.count() * 1000.0f; 
        printf("Timer took:%f ms\n",ms);
    }
};