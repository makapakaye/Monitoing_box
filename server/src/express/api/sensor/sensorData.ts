/**
 * 獲取指定感測器資料
 * 
 * 參數:
 * sensor_id (number)       感測器 ID
 * [limit_value] (number)   獲取資料量 (default: 10)
 */
export const path = '/api/sensor/sensorData';
export const method = 'GET';
export const loginRequired = false;
export const allowPermissions = [];


import { LoadType } from '../../../@types/Express.types.js';

import type { Request, Response } from 'express';
import type { Database } from '../../../lib/database/Maria.js';
import type { ApiConfig } from '../../../@types/Config.types.js';
import type { ResultData } from '../../../@types/Express.types.js';


export async function execute(req: Request, res: Response, config: ApiConfig, db: Database): Promise<ResultData> {
    let result: object[] = [];

    // 檢查請求參數型別是否正確
    if (!Number.isInteger(Number(req.query.sensor_id)) || Number(req.query.sensor_id) === 0) {
        return {
            loadType: LoadType.PARAMETER_ERROR,
            data: []
        };
    }


    const sensor_id = req.query.sensor_id;
    let limitValue = 10;

    if (req.query.limit_value && Number.isInteger(Number(req.query.limit_value))) {
        limitValue = Number(req.query.limit_value);
    }


    try {
        const query = `
            SELECT 
                temperature, humidity, pressure, gas_resistance, created_at
            FROM 
                SensorData
            WHERE 
                sensor_id = ${sensor_id}
            ORDER BY created_at DESC 
            LIMIT ${limitValue};
        `;
        result = await db.query(query);
        console.log(path, result);
    } catch (error) {
        console.log(path, error);
        return {
            loadType: LoadType.QUERY_FAILED,
            data: []
        };
    }


    /**
     * 資料庫 TIMESTAMP 獲取的時區為 UTC (+0:00) (2024-06-23T05:05:05.000Z)
     * 需轉換成 UTC+8 (2024-06-23 13:05:05)
     * 不在 DB 端處理時區轉換, server 端處理就好
     */
    result = result.map((item: any) => {
        item.created_at = new Date(item.created_at as string).toLocaleString('zh-TW', {
            year: 'numeric',
            month: '2-digit',
            day: '2-digit',
            hour12: false,
            hour: '2-digit',
            minute: '2-digit',
            second: '2-digit',
        });
        return item;
    });


    return {
        loadType: LoadType.SUCCEED,
        data: result
    };
}