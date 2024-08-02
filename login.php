
<?php
session_start();

include $_SERVER['DOCUMENT_ROOT'] . "/db.php";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $userID = $_POST["userID"];
    $password = $_POST["password"];

    if (empty($userID) || empty($password)) {
        $error = "아이디와 비밀번호를 모두 입력하세요.";
    } else {
        // 입력받은 아이디로 데이터베이스에서 사용자 정보를 가져옴
        $result = mq("SELECT * FROM opgg.user WHERE userID='$userID'");

        if ($result->num_rows > 0) {
            $row = $result->fetch_assoc();
            $Password = $row["password"];

            // 입력받은 비밀번호와 데이터베이스에 저장된 해시된 비밀번호를 비교
            if ($password == $Password) {
                // 비밀번호가 일치하는 경우 세션에 사용자 정보 저장
                $_SESSION["userID"] = $userID;
                $_SESSION["nickname"] = $nickname;
                header("Location: index.php");
                exit();
            } else {
                $error = "비밀번호가 일치하지 않습니다.";
            }
        } else {
            $error = "존재하지 않는 아이디입니다.";
        }
    }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login Page</title>
    <link rel="stylesheet" href="/css/login.css?after"/>
</head>
<body style="overflow-x: hidden; overflow-y: hidden;">
<div class="backgroundImage"></div>
  <!--로그인 헤더-->
    <div class="header">
        <div class="contect">
        Contact us <strong>watergon1126@naver.com</strong> 
        </div>
    </div>
    <div class="header">
        <div class="logo">
            <a href="/"><img src="img/logoW.png" alt="OP.GG" height="32"></a>
        </div>
    </div>
    <div>
        <div class = "hello">
            <h1>저희와 함께해주셔서</h1>
            <h1>진심으로 감사합니다.</h1> </br>
            <h4>생명을 구하기 위해</h4>
            <h4>우리 펫 같이 구조에 등록을 완료하고</h4>
            <h4>활동을 시작하세요.</h4>
        </div>
        <div class ="textbox">
            <div class="loginbox">
                <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
                <label for="userID">아이디</label></br>
                <input type="text" id="userID" name="userID"><br><br>
                    
                <label for="password">비밀번호</label></br>
                <input type="password" id="password" name="password"><br><br>
                <button type="submit">계속하기</button></br></br>
                </form>
            </div>
            <div class="signupbox">
                <h4>계정이 없으신가요?</h4>
                <a href="/signup.php"><button>회원가입</button></a>
            </div>
            <?php
            if (isset($error)) {
                echo "<p style='color:red;'>$error</p>";
            }
            ?>
        </div>
    </div>
</body>
</html>