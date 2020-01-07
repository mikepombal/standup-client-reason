let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let authLink = {
  let authHandler = () => {
    let token = Storage.getTokenFromStorage();

    switch (token) {
    | Some(t) => {
        "headers": {
          "Authorization": {j|Bearer $t|j},
        },
      }
    | None => {
        "headers": {
          "Authorization": "",
        },
      }
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