let saveUserToStorage = authorizeWithGithub => {
  let role =
    switch (authorizeWithGithub##user##role) {
    | `SUPERADMIN
    | `ADMIN => "admin"
    | _ => "basic"
    };
  open Dom.Storage;
  localStorage |> setItem("username", authorizeWithGithub##user##username);
  localStorage |> setItem("firstname", authorizeWithGithub##user##firstname);
  localStorage |> setItem("role", role);
  localStorage |> setItem("token", authorizeWithGithub##token);
};

let getTokenFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("token"));
};

let getUsernameFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("username"));
};

let getRoleFromStorage = () => {
  Dom.Storage.(localStorage |> getItem("role"));
};

let removeTokenFromStorage = () => {
  Dom.Storage.(localStorage |> removeItem("token"));
};