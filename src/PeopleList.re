module LastUpdateQueryConfig = [%graphql
  {|
  query LastUpdate {
    getLastUpdate {
      username,
      firstname,
    }
  }
|}
];

module LastUpdate = ReasonApolloHooks.Query.Make(LastUpdateQueryConfig);

[@react.component]
let make = () => {
  let (_, full) = LastUpdate.use();
  <div>
    {switch (full) {
     | {loading: true} => <p> {ReasonReact.string("Loading")} </p>
     | {data} =>
       switch (data) {
       | Some(d) =>
         <ul>
           {ReasonReact.array(
              Array.map(
                item =>
                  <Person
                    key={item##username}
                    username={item##username}
                    firstname={item##firstname}
                  />,
                d##getLastUpdate,
              ),
            )}
         </ul>
       | None => <p> {ReasonReact.string("No Data")} </p>
       }
     }}
  </div>;
};