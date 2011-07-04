%% ---
%%  Excerpted from "Programming Erlang",
%%  published by The Pragmatic Bookshelf.
%%  Copyrights apply to this code. It may not be used to create training material, 
%%  courses, books, articles, and the like. Contact us if you are in doubt.
%%  We make no guarantees that this code is fit for any purpose. 
%%  Visit http://www.pragmaticprogrammer.com/titles/jaerlang for more book information.
%%---
-module(http).
-compile(export_all).
-import(lists, [reverse/1]).


nano_client_eval(Str) ->
    {ok, Socket} = 
	gen_tcp:connect("localhost", 2345,
			[binary, {packet, 4}]),
    ok = gen_tcp:send(Socket, term_to_binary(Str)),
    receive
	{tcp,Socket,Bin} ->
	    io:format("Client received binary = ~p~n",[Bin]),
	    Val = binary_to_term(Bin),
	    io:format("Client result = ~p~n",[Val]),
	    gen_tcp:close(Socket)
    end.

% {ok, Socket} = gen_tcp:accept(Listen),
% inet:setopts(Socket, [{packet,4},binary,
% 				{nodelay,true},{active, true}]),

start_nano_server() ->
    {ok, Listen} = gen_tcp:listen(2345, [binary, {packet, 4},  %% (6)
					 {reuseaddr, true},
					 {active, true}]),
    {ok, Socket} = gen_tcp:accept(Listen),  %% (7)
    gen_tcp:close(Listen),  %% (8)
    loop(Socket).

start_seq_server() ->
	{ok, Listen} = gen_tcp:listen(2345, [binary, {packet, 4},  %% (6)
					 {reuseaddr, true},
					 {active, once}]),    %change to true, false, once
	seq_loop(Listen).

seq_loop(Listen) ->
{ok, Socket} = gen_tcp:accept(Listen),
	loop(Socket),
	seq_loop(Listen).

start_parallel_server() ->
	{ok, Listen} = gen_tcp:listen(2345, [binary, {packet, 4},  %% (6)
					 {reuseaddr, true},
					 {active, true}]),
	spawn(fun() -> par_connect(Listen) end).

par_connect(Listen) ->
	{ok, Socket} = gen_tcp:accept(Listen),
	spawn(fun() -> par_connect(Listen) end),
	loop(Socket).

loop(Socket) -> % hybrid
	receive
	{tcp, Socket, Bin} ->
	    io:format("Server received binary = ~p~n",[Bin]),
	    Str = binary_to_term(Bin),
	    io:format("Server (unpacked)  ~p~n",[Str]),
	    Reply = lib_misc:string2value(Str),
	    io:format("Server replying = ~p~n",[Reply]),
	    gen_tcp:send(Socket, term_to_binary(Reply)),
	    %% when you're ready enable the next message
	    inet:setopts(Socket, [{active, once}]),
	    loop(Socket);
	{tcp_closed, Socket} ->
	    io:format("Server socket closed~n")
	end.

loop1(Socket) -> % nonblocking
    receive
	{tcp, Socket, Bin} ->
	    io:format("Server received binary = ~p~n",[Bin]),
	    Str = binary_to_term(Bin),  %% (9)
	    io:format("Server (unpacked)  ~p~n",[Str]),
	    Reply = lib_misc:string2value(Str),  %% (10)
	    io:format("Server replying = ~p~n",[Reply]),
	    gen_tcp:send(Socket, term_to_binary(Reply)),  %% (11)
	    loop(Socket);
	{tcp_closed, Socket} ->
	    io:format("Server socket closed~n")
    end.

loop2(Socket) -> % blocking
	case gen_tcp:recv(Socket, 0) of   % maybe N = 9000;
		{ok, Bin} ->
			io:format("Server received binary = ~p~n",[Bin]),
			Str = binary_to_term(Bin),  %% (9)
			io:format("Server (unpacked)  ~p~n",[Str]),
			Reply = lib_misc:string2value(Str),  %% (10)
			io:format("Server replying = ~p~n",[Reply]),
			gen_tcp:send(Socket, term_to_binary(Reply)),  %% (11)
			loop(Socket);
		{error, closed} ->
			io:format("Server socket closed~n");
		{error, einval} ->
			io:format("Server mean mist~n")
	end.

%%========================================================================

nano_get_url() ->
    nano_get_url("www.google.com", 80).

nano_get_url(Host, Port) ->
    {ok,Socket} = gen_tcp:connect(Host,Port,[binary, {packet, 0}]), %% (1)
    ok = gen_tcp:send(Socket, "GET / HTTP/1.0\r\n\r\n"),  %% (2)
    receive_data(Socket, []).

receive_data(Socket, SoFar) ->
    receive
	{tcp,Socket,Bin} ->    %% (3)
	    receive_data(Socket, [Bin|SoFar]);
	{tcp_closed,Socket} -> %% (4)
	    list_to_binary(reverse(SoFar)) %% (5)
    end.



error_test() ->
    spawn(fun() -> error_test_server() end),
    lib_misc:sleep(2000),
    {ok,Socket} = gen_tcp:connect("localhost",4321,[binary, {packet, 2}]),
    io:format("connected to:~p~n",[Socket]),
    gen_tcp:send(Socket, <<"123">>),
    receive
	Any ->
	    io:format("Any=~p~n",[Any])
    end.

error_test_server() ->
    {ok, Listen} = gen_tcp:listen(4321, [binary,{packet,2}]),
    {ok, Socket} = gen_tcp:accept(Listen),
    error_test_server_loop(Socket).

error_test_server_loop(Socket) ->
    receive
	{tcp, Socket, Data} ->
	    io:format("received:~p~n",[Data]),
	    A = atom_to_list(Data),
	    io:format("~p~n", A),
	    error_test_server_loop(Socket)
    end.
