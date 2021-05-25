modded class TerritoryFlag {
    ref _Event _registeredInstance = new _Event("TerritoryFlag", "pennant", this);

    override void InitRefresherData() {
        super.InitRefresherData();
        if (GetGameLabs()) {
            if (GetGameLabs().IsServer()) {
                GetGameLabs().RegisterEvent(this._registeredInstance);
            }
        }
    }

    override void RemoveRefresherPosition(int idx = -2) {
        super.RemoveRefresherPosition(idx);
        if(GetGameLabs()) {
            if(GetGameLabs().IsServer()) {
                if(this._registeredInstance) GetGameLabs().RemoveEvent(this._registeredInstance);
            }
        }
    }

    // Public API
    _Event _GetEventInstance() {
        return this._registeredInstance;
    }

    void _SetEventInstance(ref _Event _registeredInstance) {
        this._registeredInstance = _registeredInstance;
    }
};