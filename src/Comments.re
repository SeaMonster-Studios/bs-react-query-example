[@decco]
type comment = {
  id: string,
  postId: string,
  body: string,
  authorId: string,
};

[@decco]
type comments = array(comment);

module Queries = {
  module Query = {
    include ReactQuery.Query({
      type key = string;
      type data = Belt.Result.t(comments, Decco.decodeError);
      type vars = unit;
    });

    let getEndpoint = postId => {j|/api/comments/?postId=$postId|j};

    let use = (postId: option(string)) => {
      use(
        ~key=postId->Belt.Option.map(postId => postId->getEndpoint), (key, _) =>
        Js.Promise.(
          Fetch.fetch(key)
          |> then_(Fetch.Response.json)
          |> then_(json => json->comments_decode->resolve)
        )
      );
    };
  };

  module Create = {
    include ReactQuery.Mutation({
      type data = Belt.Result.t(comment, Decco.decodeError);
      type vars = comment;
    });

    let use = () => {
      use(
        ~options=
          Options.(
            make(
              ~onSuccess=
                (data, _) => {
                  data->Belt.Result.mapWithDefault(
                    (),
                    comment => {
                      let default =
                        [|comment|]->Belt.Result.Ok->Js.Nullable.return;
                      Query.Cache.setData(
                        comment.postId->Query.getEndpoint, comments =>
                        comments
                        ->Js.Nullable.toOption
                        ->Belt.Option.mapWithDefault(default, comments =>
                            comments->Belt.Result.mapWithDefault(
                              default, comments =>
                              [|comment|]
                              ->Belt.Array.concat(comments)
                              ->Belt.Result.Ok
                              ->Js.Nullable.return
                            )
                          )
                      );
                    },
                  );
                  Js.Nullable.null;
                },
              (),
            )
          ),
        comment =>
        Js.Promise.(
          Fetch.fetchWithInit(
            "/api/comments",
            Fetch.RequestInit.make(
              ~method_=Post,
              ~headers=
                Fetch.HeadersInit.make({
                  "Content-Type": "application/json",
                  "Accept": "application/json",
                }),
              ~body=
                Fetch.BodyInit.make(
                  comment->comment_encode->Js.Json.stringify,
                ),
              (),
            ),
          )
          |> then_(Fetch.Response.json)
          |> then_(json => json->comment_decode->resolve)
        )
      );
    };
  };
};