let saveTokenToStorage = value => {
  Dom.Storage.(localStorage |> setItem("token", value));
};

let getTokenFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("token"));
};

let saveUserToStorage = user => {
  open Dom.Storage;
  localStorage |> setItem("username", user##username);
  localStorage |> setItem("firstname", user##firstname);
};

let getUserFromStorage = () => {
  (
    Dom.Storage.(localStorage |> getItem("username")),
    Dom.Storage.(localStorage |> getItem("bio")),
    Dom.Storage.(localStorage |> getItem("image")),
  );
};