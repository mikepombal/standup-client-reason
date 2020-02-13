let saveUserToStorage = authorizeWithGithub => {
  open Dom.Storage;
  localStorage |> setItem("username", authorizeWithGithub##user##username);
  localStorage |> setItem("firstname", authorizeWithGithub##user##firstname);
  localStorage |> setItem("token", authorizeWithGithub##token);
};

let getTokenFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("token"));
};

let getUsernameFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("username"));
};

let removeTokenFromStorage = () => {
  Dom.Storage.(localStorage |> removeItem("token"));
};