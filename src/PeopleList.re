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

module AddUpdatesMutationConfig = [%graphql
  {|
    mutation addUpdates($date: String!, $people: [String!]!) {
        addUpdates(date: $date, usrnames: $people)
    }
|}
];

module LastUpdate = ReasonApolloHooks.Query.Make(LastUpdateQueryConfig);
module AddUpdates = ReasonApolloHooks.Mutation.Make(AddUpdatesMutationConfig);

type action =
  | TogglePerson(string);

[@react.component]
let make = () => {
  let (_, full) = LastUpdate.use();
  let (addUpdates, _, _) = AddUpdates.use();
  let (date, setDate) = React.useState(() => Helpers.getCurrentDateString());
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
  let onSubmit = () => {
    let variables =
      AddUpdatesMutationConfig.make(
        ~date,
        ~people=Array.of_list(listSelected),
        (),
      )##variables;
    addUpdates(~variables, ())
    |> Js.Promise.then_(
         (
           res:
             ReasonApolloHooks.Mutation.controlledVariantResult(
               AddUpdatesMutationConfig.t,
             ),
         ) => {
         Js.log(res);
         Js.Promise.resolve();
       })
    |> ignore;
  };
  <div>
    {switch (full) {
     | {loading: true} => <p> {ReasonReact.string("Loading")} </p>
     | {data} =>
       switch (data) {
       | Some(d) =>
         <>
           <ul>
             {ReasonReact.array(
                Array.map(
                  item =>
                    <Person
                      key={item##username}
                      firstname={item##firstname}
                      checked={
                        switch (
                          List.find(
                            str => str === item##username,
                            listSelected,
                          )
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
           <input
             type_="date"
             value=date
             onChange={evt => setDate(ReactEvent.Form.target(evt)##value)}
           />
           <button
             onClick={_evt => onSubmit()}
             disabled={List.length(listSelected) == 0}>
             {ReasonReact.string("Add Updates")}
           </button>
           <code>
             {ReasonReact.array(
                Array.of_list(
                  List.map(
                    username =>
                      ReasonReact.string("\"" ++ username ++ "\", "),
                    listSelected,
                  ),
                ),
              )}
           </code>
         </>
       | None => <p> {ReasonReact.string("No Data")} </p>
       }
     }}
  </div>;
};