/*
 * GameLabs core web api
 *
 * REST style is not available due to header limitations
 */

class GameLabsAPI {
    private static const string baseUrl = "https://api.cftools.local/gamelabs/dz";

    private ref RestApi restApi;
    private ref RestContext restContext;
    private ref JsonSerializer jsonSerializer = new JsonSerializer();

    private string serverId;
    private string apiKey;
    private string authKey;

    private bool active = false;

    void GameLabsAPI(string serverId, string apiKey) {
        this.serverId = serverId;
        this.apiKey = apiKey;

        this.restApi = CreateRestApi();
        this.restContext = this.restApi.GetRestContext(this.baseUrl);

        this.restApi.EnableDebug(false);
    }

    void Enable() { this.active = true; }
    void Disable() { this.active = false; }
    bool IsEnabled() { return this.active; }

    int Register() {
        this.restContext.SetHeader("application/json"); // Expected for auth request

        ref _Payload_Register payload = new ref _Payload_Register(this.serverId, this.apiKey);
        ref _Response_Register response = new _Response_Register(this.restContext.POST_now("/v1/auth/register", payload.ToJson()));
        if(response.status == 2) {
            GetGameLabs()._PropagateFeatures(response); // Access features outside of API class

            this.authKey = response.authKey;
            this.restContext.SetHeader(this.authKey); // Currently setting to Content-Type; DZ does not allow to set custom headers
            GetGameLabs().GetLogger().Debug(string.Format("authKey=%1", this.authKey));
        }
        return response.status;
    }

    int Verify() {
        if(!this.IsEnabled()) return -1;

        ref _Payload payload = new ref _Payload();
        _Response response = new _Response(this.restContext.POST_now("/v1/auth/verify", payload.ToJson()));
        return response.status;
    }

    void ServerPoll(ref Managed cb, ref _Payload_ServerPoll payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/server/poll", payload.ToJson());
    }

    void ServerEvents(ref Managed cb, ref _Payload_ServerEvents payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/server/events", payload.ToJson());
    }

    void ServerVehicles(ref Managed cb, ref _Payload_ServerVehicles payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/server/vehicles", payload.ToJson());
    }

    void ServerPlayers(ref Managed cb, ref _Payload_ServerPlayers payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/server/players", payload.ToJson());
    }

    void PlayerDeath(ref Managed cb, ref _Payload_PlayerDeath payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/player/death", payload.ToJson());
    }

    void PlayerDamage(ref Managed cb, ref _Payload_PlayerDamage payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/player/damage", payload.ToJson());
    }

    void ItemInteract(ref Managed cb, ref _Payload_ItemInteract payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/item/interact", payload.ToJson());
    }

    void ItemPlace(ref Managed cb, ref _Payload_ItemPlace payload) {
        if(!this.IsEnabled()) return;
        this.restContext.POST(cb, "/v1/item/place", payload.ToJson());
    }
};