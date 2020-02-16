let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let authLink = {
  let authHandler = () => {
    let username =
      switch (Storage.getUsernameFromStorage()) {
      | Some(t) => t
      | None => ""
      };
    let authorization =
      switch (Storage.getTokenFromStorage()) {
      | Some(t) => {j|Bearer $t|j}
      | None => ""
      };

    {
      "headers": {
        "Authorization": authorization,
        "Username": username,
      },
    };
  };

  ApolloLinks.createContextLink(authHandler);
};

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://standup-server.herokuapp.com/",
    (),
  );

let errorLink =
  ApolloLinks.createErrorLink(errorResponse => {
    let messages =
      errorResponse##graphQLErrors
      ->Js.Nullable.toOption
      ->Belt.Option.flatMap(error =>
          Some(error |> Array.map(err => err##message))
        );
    let isTokenNotValid =
      switch (messages) {
      | Some(a) => Belt.Array.some(a, s => s == "Token not valid")
      | _ => false
      };
    if (isTokenNotValid) {
      Storage.removeTokenFromStorage();
      Route.push(Login);
    };
  });

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, errorLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );