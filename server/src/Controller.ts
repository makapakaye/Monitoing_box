import { App } from "./express/App.js";
import { Authorizer } from "./lib/SessionManager/Authorizer.js";
import { Database } from "./lib/database/Maria.js";
import { SessionManager } from "./lib/SessionManager/SessionManager.js";
import { loadEnviornment } from "./loader/loadEnviornment.js";

import { config } from "../config.js";

import type { Config } from "./@types/Config.types.js";


export class Controller {
    public config: Config;
    public app?: App;

    #db: Database;
    #sessionManager: SessionManager;


    constructor() {
        this.config = config;
        const env = loadEnviornment();

        this.#db = new Database(env.dbConfig);
        this.#sessionManager = new SessionManager(new Authorizer(env.salt, this.#db), this.#db, this.config.ipBlocker);
        this.app = new App(this.config.apiConfig, this.#db, this.#sessionManager);
    }


    /**
     * 啟動 express 框架
     */
    public async initializeExpress(): Promise<void> {
        await this.app?.setRoutes();
        this.app?.startListening();
    }
}