type unsplashConfig = {.
  "applicationId": string,
  "secret": string,
  "callbackUrl": string
};
type unsplash = {.
  "collections": {.
    [@bs.meth] "listCollections": (int, int, string) => Js.Promise.t(Fetch.Response.t)
  },
  "photos": {.
    [@bs] [@bs.meth] "listPhotos": (int, int, string) => Js.Promise.t(Fetch.Response.t)
  }
};
[@bs.new] [@bs.module "unsplash-js/native"] external createUnsplashClient : unsplashConfig => unsplash = "default";

let appId = Environment.appId;
let secret = Environment.secret;
let callbackUrl = "urn:ietf:wg:oauth:2.0:oob";

let unsplash = createUnsplashClient({
  "applicationId": appId,
  "secret": secret,
  "callbackUrl": callbackUrl
});

let listPhotos = unsplash##photos##listPhotos;
