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
    // ~uri="https://standup-server.herokuapp.com/",
    ~uri="http://localhost:4000",
    (),
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );