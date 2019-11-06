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

type action =
  | TogglePerson(string);

[@react.component]
let make = () => {
  let (_, full) = LastUpdate.use();
  let (listSelected, dispatch) =
    React.useReducer(
      (l, action) =>
        switch (action) {
        | TogglePerson(person) =>
          switch (List.find(str => str === person, l)) {
          | exception Not_found => [person, ...l]
          | _ => List.filter(str => str !== person, l)
          }
        },
      [],
    );
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
                    firstname={item##firstname}
                    checked={
                      switch (
                        List.find(str => str === item##username, listSelected)
                      ) {
                      | exception Not_found => false
                      | _ => true
                      }
                    }
                    toggle={_evt => dispatch(TogglePerson(item##username))}
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